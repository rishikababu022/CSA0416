#!/usr/bin/env bash
# B1.3 - allow only SSH, HTTP and HTTPS. Prints the commands; run with APPLY=1 (as root) to execute them.
run() { if [ "${APPLY:-0}" = 1 ]; then "$@"; else echo "+ $*"; fi; }
run ufw default deny incoming
run ufw default allow outgoing
run ufw allow 22/tcp
run ufw allow 80/tcp
run ufw allow 443/tcp
run ufw --force enable
run ufw status verbose
# iptables equivalent:
#   iptables -P INPUT DROP ; iptables -A INPUT -i lo -j ACCEPT
#   iptables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
#   iptables -A INPUT -p tcp -m multiport --dports 22,80,443 -j ACCEPT
