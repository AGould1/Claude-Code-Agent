# Claude Code Agent

An AI coding agent built in C++ that communicates with a Large Language Model (LLM) via the OpenRouter API. Supports an autonomous agent loop with Read, Write, and Bash tools — enabling the model to read files, write code, and execute shell commands on your local machine through a browser-based chat UI.

## Features

- **Agent Loop** — continuously sends messages to the LLM and handles tool calls until a final response is ready
- **Read Tool** — allows the LLM to read files on your filesystem
- **Write Tool** — allows the LLM to create or overwrite files
- **Bash Tool** — allows the LLM to execute shell commands
- **Chat UI** — dark mode browser interface with message history and markdown rendering
- **Persistent Chat History** — conversations are saved between sessions
- **One-Click Launch** — batch script to start everything automatically

## Tech Stack

- **C++** — core agent logic and LLM communication
- **Python / Flask** — local server bridging the UI and the C++ agent
- **HTML / CSS / JavaScript** — browser-based chat interface
- **OpenRouter API** — LLM access (Claude Haiku)
- **nlohmann/json** — JSON parsing in C++
- **cpr** — HTTP requests in C++

## Requirements

- Windows (MSYS2 with UCRT64)
- Python 3 with Flask and flask-cors (`pip install flask flask-cors`)
- An [OpenRouter](https://openrouter.ai) API key
- MSYS2 with gcc, cpr, and nlohmann-json installed

## Setup

1. Clone the repository
2. Compile the C++ agent in MSYS2:
   ```bash
   g++ src/main.cpp -o claude-agent.exe -I/ucrt64/include -L/ucrt64/lib -lcpr -lcurl
   ```
3. Add your OpenRouter API key to `launch.bat`
4. Double-click `launch.bat` to start the server and open the UI

## Usage

Once launched, open your browser to `http://localhost:5000` and start chatting. The agent can:

- Answer general questions
- Read files on your computer
- Create and edit files
- Run terminal commands

## Notes

- Files are created relative to the `claude-agent` directory by default
- Chat history is saved to `chat_history.json` and loaded on startup
- Delete `chat_history.json` to start a fresh conversation
