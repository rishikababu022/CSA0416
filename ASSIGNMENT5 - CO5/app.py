"""Sample campus result-portal (stand-in for the university's Python web application)."""
import hashlib
import os
import socket
import time

from flask import Flask, jsonify

app = Flask(__name__)
SERVICE = os.environ.get("SERVICE_NAME", "result-portal")
DB_HOST = os.environ.get("DB_HOST", "")
START = time.time()


def _cpu_work(regno: str, rounds: int = 2000) -> str:
    """About 1 ms of CPU work per request, so the portal is CPU-bound like a real result look-up."""
    h = regno.encode()
    for _ in range(rounds):
        h = hashlib.sha256(h).digest()
    return h.hex()[:8]


@app.route("/")
def index():
    return f"{SERVICE}: semester results. Use /result/<register-number>\n"


@app.route("/health")
def health():
    return jsonify(status="ok", service=SERVICE, uptime_s=round(time.time() - START, 1))


@app.route("/result/<regno>")
def result(regno):
    grade = "ABCDE"[int(_cpu_work(regno), 16) % 5]
    return jsonify(regno=regno, grade=grade)


@app.route("/dbcheck")
def dbcheck():
    if not DB_HOST:
        return jsonify(db="not configured")
    try:
        with socket.create_connection((DB_HOST, 5432), timeout=2):
            pass
        return jsonify(db="reachable", host=DB_HOST)
    except OSError as exc:
        return jsonify(db="unreachable", error=str(exc)), 503


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=int(os.environ.get("PORT", "8000")), threaded=True)
