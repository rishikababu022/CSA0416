#!/usr/bin/env bash
# B3 alternative concept: Docker Compose scaling on one server (no host port in the compose file, nginx in front).
set -x
docker compose -f B3_compose_scale.yml up -d --scale portal=6
docker compose -f B3_compose_scale.yml ps
docker kill "$(docker compose -f B3_compose_scale.yml ps -q portal | head -n 1)"   # failed container
sleep 2; docker compose -f B3_compose_scale.yml ps    # Compose does NOT recreate it automatically unless restart: policy is set
docker compose -f B3_compose_scale.yml down
