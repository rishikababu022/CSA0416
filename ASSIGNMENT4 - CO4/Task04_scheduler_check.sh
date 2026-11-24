#!/bin/sh
# Task 4.4: show the I/O scheduler and the rotational flag of each block device.
echo "Device  rotational(1=HDD,0=SSD)  scheduler"
for d in /sys/block/*; do
    n=$(basename "$d")
    [ -r "$d/queue/scheduler" ] || continue
    rot=$(cat "$d/queue/rotational" 2>/dev/null)
    printf "%-7s %-24s %s\n" "$n" "$rot" "$(cat "$d/queue/scheduler")"
done
echo "(the scheduler in [brackets] is the active one; change with: echo mq-deadline | sudo tee /sys/block/<disk>/queue/scheduler)"
