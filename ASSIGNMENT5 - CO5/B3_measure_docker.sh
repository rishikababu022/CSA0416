#!/usr/bin/env bash
# B3.1/B3.2 - container deployment measurements (needs a running Docker daemon).
set -u
docker build -q -f B2_Dockerfile -t result-portal . >/dev/null
T0=$(date +%s.%N)
docker run -d --name portal-m -p 8000:8000 result-portal >/dev/null
python3 B3_loadtest.py http://127.0.0.1:8000/health --wait
echo "wall time from docker run to ready: $(echo "$(date +%s.%N) - $T0" | bc) s"
echo "### image size"; docker images result-portal --format '{{.Repository}}:{{.Tag}} {{.Size}}'
for c in 1 8 32; do
    echo "### load: concurrency $c, 10 s"
    python3 B3_loadtest.py http://127.0.0.1:8000/result/ABC123 --concurrency $c --duration 10
done
echo "### memory"; docker stats --no-stream --format '{{.Name}} {{.MemUsage}} {{.CPUPerc}}' portal-m
docker rm -f portal-m >/dev/null
