from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import os
import subprocess

app = Flask(__name__)
CORS(app)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/chat', methods=['POST'])
def chat():
    api_key = os.environ.get("OPENROUTER_API_KEY")
    data = request.json
    message = data["message"]
    result = subprocess.run(["C:/dev/claude-agent/claude-agent.exe", "-p", message], env={**os.environ, "OPENROUTER_API_KEY": api_key}, capture_output=True, text=True, encoding='utf-8')
    print("stdout:", result.stdout)
    print("stderr:", result.stderr)
    print("returncode:", result.returncode)
    return jsonify({"response": result.stdout})

@app.route('/history', methods=['GET'])
def history():
    if(os.path.exists("chat_history.json")):
        with open("chat_history.json") as output:
            data = output.read()
            return jsonify({"history": data})
    else:
        return jsonify({"history": []})

if __name__ == '__main__': 
    app.run(debug=True)