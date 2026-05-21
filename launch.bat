set OPENROUTER_API_KEY=your-api-key-here
set PATH=C:\msys64\ucrt64\bin;%PATH%

start python server.py
timeout /t 2
start http://localhost:5000