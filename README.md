# Claude Code Agent (C++)

A minimal AI coding agent built in C++ that communicates with an LLM via the OpenRouter API. Supports an agent loop with Read, Write, and Bash tools. This enables the model to read files, write code, and execute shell commands autonomously.

## What it does
- Sends user prompts to an LLM (Claude Haiku via OpenRouter)
- Implements an agent loop that runs until the LLM has a final answer
- Supports three tools:
  - Read — read the contents of a file
  - Write — create or overwrite a file
  - Bash — execute shell commands

## How it works
The program maintains a conversation history (`messages`) that grows with each iteration. The LLM can request tools, your program executes them and appends the results, and the loop continues until the LLM responds with a final text answer.

## Usage
```bash
./your_program.sh -p "Your prompt here"
```

## Built with
- C++
- [cpr](https://github.com/libcpr/cpr) — HTTP requests
- [nlohmann/json](https://github.com/nlohmann/json) — JSON parsing
- OpenRouter API

## Based on
[CodeCrafters — Build Your Own Claude Code](https://codecrafters.io)
