#!/usr/bin/env bash
# B1.3 - service account, directories and systemd/nginx installation for the portal.
# Prints the commands by default; run with APPLY=1 as root on a Debian/Ubuntu server to execute.
run() { if [ "${APPLY:-0}" = 1 ]; then "$@"; else echo "+ $*"; fi; }
run groupadd --system portal
run useradd --system --gid portal --home-dir /opt/portal --shell /usr/sbin/nologin portal
run install -d -o portal -g portal /opt/portal /var/log/portal
run install -m 644 app.py requirements.txt /opt/portal/
run python3 -m venv /opt/portal/venv
run /opt/portal/venv/bin/pip install -r /opt/portal/requirements.txt
run install -m 644 B1_portal.service /etc/systemd/system/portal.service
run install -m 644 B1_nginx_portal.conf /etc/nginx/sites-available/portal
run ln -sf /etc/nginx/sites-available/portal /etc/nginx/sites-enabled/portal
run install -m 755 B1_log_summary.sh B1_disk_alert.sh /opt/portal/
run install -m 644 B1_portal.logrotate /etc/logrotate.d/portal
run systemctl daemon-reload
run systemctl enable --now portal
run nginx -t
run systemctl reload nginx
run systemctl status portal --no-pager
