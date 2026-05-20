#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "-p") {
        std::cerr << "Expected first argument to be '-p'" << std::endl;
        return 1;
    }

    std::string prompt = argv[2];

    if (prompt.empty()) {
        std::cerr << "Prompt must not be empty" << std::endl;
        return 1;
    }

    const char* api_key_env = std::getenv("OPENROUTER_API_KEY");
    const char* base_url_env = std::getenv("OPENROUTER_BASE_URL");

    std::string api_key = api_key_env ? api_key_env : "";
    std::string base_url = base_url_env ? base_url_env : "https://openrouter.ai/api/v1";

    if (api_key.empty()) {
        std::cerr << "OPENROUTER_API_KEY is not set" << std::endl;
        return 1;
    }

    json messages = json::array({
        {{"role", "user"}, {"content", prompt}}
    });

    while (true) {
        json request_body = {
        {"model", "anthropic/claude-haiku-4.5"},
        {"messages", messages},
        {"tools", json::array({
            {{"type", "function"}, {"function", {
                {"name", "Read"}, 
                {"description", "Read and return the contents of a file"},
                {"parameters", {
                    {"type", "object"}, 
                    {"properties", {
                        {"file_path", {
                            {"type", "string"}, 
                            {"description", "The path to the file to read"}
                        }}
                    }},
                    {"required", json::array({"file_path"})}
                }}
            }}},
            {{"type", "function"}, {"function", {
                {"name", "Write"},
                {"description", "Write content to a file"},
                {"parameters", {
                    {"type", "object"},
                    {"properties", {
                        {"file_path", {
                            {"type", "string"},
                            {"description", "The path of the file to write to"}
                        }},
                        {"content", {
                            {"type", "string"},
                            {"description", "The content to write to the file"}
                        }}
                    }},
                    {"required", 
                        json::array({"file_path", "content"})
                    }
                }}
            }}},
            {{"type", "function"}, {"function", {
                {"name", "Bash"},
                {"description", "Execute a shell command"},
                {"parameters", {
                    {"type", "object"},
                    {"properties", {
                        {"command", {
                            {"type", "string"},
                            {"description", "The command to execute"}
                        }}
                    }},
                    {"required",
                        json::array({"command"})
                    }
                }}
            }}}
        })}
    };

        cpr::Response response = cpr::Post(
        cpr::Url{base_url + "/chat/completions"},
        cpr::Header{
            {"Authorization", "Bearer " + api_key},
            {"Content-Type", "application/json"}
        },
        cpr::Body{request_body.dump()}
    );

    if (response.status_code != 200) {
        std::cerr << "HTTP error: " << response.status_code << std::endl;
        return 1;
    }

    json result = json::parse(response.text);

    if (!result.contains("choices") || result["choices"].empty()) {
        std::cerr << "No choices in response" << std::endl;
        return 1;
    }

    messages.push_back(result["choices"][0]["message"]);

    if (result["choices"][0]["message"].contains("tool_calls")) {
        auto tool_calls = result["choices"][0]["message"]["tool_calls"][0];
        std::string function_name = tool_calls["function"]["name"];
        std::string function_arguments = tool_calls["function"]["arguments"];

        json arguments = json::parse(function_arguments);

        if (function_name == "Read") {
            std::string file_path = arguments["file_path"];

            std::ifstream file(file_path);
            if (!file.is_open()) {
                std::cerr << "Error opening file" << std::endl;
                return 1;
            }
            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            messages.push_back({
                {"role", "tool"},
                {"tool_call_id", tool_calls["id"]},
                {"content", contents}
            });
        }

        else if (function_name == "Write") {
            std::string file_path = arguments["file_path"];
            std::string content = arguments["content"];

            std::ofstream file(file_path);
            if (!file.is_open()) {
                std::cerr << "Error opening file" << std::endl;
                return 1;
            }
            file << content;

            messages.push_back({
                {"role", "tool"},
                {"tool_call_id", tool_calls["id"]},
                {"content", "File written successfully"}
            });
        }

        else if (function_name == "Bash") {
            std::string command = arguments["command"];

            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Error opening pipe" << std::endl;
                return 1;
            }
            char buffer[128];
            std::string output;
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                output += buffer;
            }
            pclose(pipe);

            messages.push_back({
                {"role", "tool"},
                {"tool_call_id", tool_calls["id"]},
                {"content", output}
            });
        }
    }

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    // TODO: Uncomment the line below to pass the first stage
    if (!result["choices"][0]["message"].contains("tool_calls")) {
        std::cout << result["choices"][0]["message"]["content"].get<std::string>();
        break;
    }
}
    return 0;
}
    