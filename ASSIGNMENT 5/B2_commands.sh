#!/usr/bin/env bash
# B2 - the commands to run (needs a running Docker daemon). Record the output of each one.
set -x
docker build -f B2_Dockerfile -t result-portal .
docker images result-portal                         # image size
docker run -d --name portal -p 8000:8000 result-portal
docker ps                                           # container running
curl -s localhost:8000/health
docker exec portal whoami                           # must print: portal
docker history result-portal                        # image layers
docker rm -f portal
docker run -d --name portal-limited --cpus 0.5 --memory 128m -p 8000:8000 result-portal
python3 B3_loadtest.py http://localhost:8000/result/ABC123 --concurrency 8 --duration 10 &
sleep 5; docker stats --no-stream portal-limited    # CPU capped near 50%, memory limit shown
wait
docker rm -f portal-limited
docker build -f B2_Dockerfile.multistage -t result-portal:multistage .
docker build -f B2_Dockerfile.ubuntu -t result-portal:ubuntu .
docker images result-portal                         # compare slim / multistage / ubuntu
docker compose -f B2_docker-compose.yml up -d
docker compose -f B2_docker-compose.yml ps
docker volume ls                                    # pgdata volume survives: docker compose down; up -d
docker stats --no-stream
docker compose -f B2_docker-compose.yml down
