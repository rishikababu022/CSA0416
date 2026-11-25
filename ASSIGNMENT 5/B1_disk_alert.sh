#!/usr/bin/env bash
# B1.2 - disk-space alert. Usage: B1_disk_alert.sh [threshold-percent]   (default 80)
# Exit code 1 when any file system is at or above the threshold (so cron / monitoring can react).
set -euo pipefail
threshold=${1:-80}
status=0
while read -r fs size used avail pct mount; do
    p=${pct%\%}
    if [ "$p" -ge "$threshold" ]; then
        msg="ALERT: $mount ($fs) is ${p}% full (used $used of $size, $avail free)"
        echo "$(date '+%F %T') $msg"
        command -v logger >/dev/null 2>&1 && logger -t disk_alert "$msg"
        status=1
    fi
done < <(df -hP -x tmpfs -x devtmpfs -x overlay | awk 'NR > 1')
[ "$status" -eq 0 ] && echo "$(date '+%F %T') OK: all file systems are below ${threshold}%"
exit $status
