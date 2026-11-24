#!/usr/bin/env bash
# B3.1/B3.2 - bare-metal (directly on Linux) deployment: start-up time, memory, disk size, requests per second.
# Uses the Flask built-in server (gunicorn is installed in the container/VM deployments).
set -u
PORT=${PORT:-8000}
echo "### start-up time"
python3 app.py > /tmp/portal_bare.log 2>&1 &
PID=$!
python3 B3_loadtest.py "http://127.0.0.1:$PORT/health" --wait
echo "### memory right after start (RSS, KiB)"; ps -o rss= -p $PID
for c in 1 8 32; do
    echo "### load: concurrency $c, 10 s"
    python3 B3_loadtest.py "http://127.0.0.1:$PORT/result/ABC123" --concurrency $c --duration 10
done
echo "### memory after load (RSS, KiB)"; ps -o rss= -p $PID
echo "### disk used by the application"; du -sk app.py requirements.txt | awk '{s+=$1} END {print s " KiB (app files)"}'
echo "python packages (flask and dependencies):"; python3 - <<'PY'
import importlib.metadata as m, os
tot = 0
for name in ("flask", "werkzeug", "jinja2", "markupsafe", "itsdangerous", "click", "blinker"):
    try:
        d = m.distribution(name)
        tot += sum(os.path.getsize(d.locate_file(f)) for f in d.files if os.path.exists(d.locate_file(f)))
    except Exception:
        pass
print(f"{tot/1024:.0f} KiB")
PY
kill $PID
