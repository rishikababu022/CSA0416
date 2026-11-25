#!/usr/bin/env bash
# B1.1 - observe processes, memory, disk and network. Each section says what the numbers mean.
have() { command -v "$1" >/dev/null 2>&1; }
echo "### uptime / load average (runnable tasks averaged over 1, 5, 15 min; compare with CPU count: $(nproc))"
uptime
echo; echo "### ps aux --sort=-%mem | head  (USER PID %CPU %MEM VSZ=virtual KB RSS=resident KB STAT=state)"
ps aux --sort=-%mem | head -n 6
echo; echo "### top -bn1 | head -n 12  (us/sy/id/wa = user/system/idle/iowait CPU %)"
top -bn1 | head -n 12
echo; echo "### free -m  (available = memory usable without swapping; buff/cache = page cache)"
free -m
echo; echo "### vmstat 1 5  (r=runnable, b=blocked, si/so=swap in/out, bi/bo=blocks in/out, wa=iowait)"
vmstat 1 5
echo; echo "### iostat -x 1 2  (r/s w/s, await=ms per I/O, %util=device busy)"
if have iostat; then iostat -x 1 2; else echo "iostat not installed (sudo apt install sysstat); /proc/diskstats first lines:"; head -n 4 /proc/diskstats; fi
echo; echo "### df -h  (size, used, available per file system)"
df -h -x tmpfs -x devtmpfs | head -n 8
echo; echo "### ss -tuln  (listening TCP/UDP sockets and ports)"
if have ss; then ss -tuln; elif have netstat; then netstat -tuln; else echo "ss/netstat not installed (iproute2); listening TCP sockets from /proc/net/tcp (state 0A = LISTEN):"; awk 'NR>1 && $4=="0A" {print $2}' /proc/net/tcp; fi
echo; echo "### /proc/meminfo (first 5 lines) and /proc/loadavg"
head -n 5 /proc/meminfo; cat /proc/loadavg
