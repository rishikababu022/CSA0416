#!/bin/bash
CFLAGS="-std=c11 -Wall -Wextra"
echo "=== lsblk ==="; lsblk 2>&1 || cat /proc/partitions
echo; echo "=== fdisk -l (needs root) ==="; fdisk -l 2>&1 | head -30
echo; echo "=== /proc/partitions ==="; cat /proc/partitions 2>&1
echo; echo "=== swap ==="; swapon --show 2>&1; cat /proc/swaps 2>&1; free -h
echo; echo "=== mounts of real filesystems ==="; df -hT 2>/dev/null | head -8
echo; echo "=== RAID capacity and probability ==="
gcc $CFLAGS -o Q6_raid Q6_raid.c -lm && ./Q6_raid
echo; echo "=== Read speed (run on each device if both SSD and HDD exist; set DIR to a directory on that disk) ==="
DIR=${DIR:-.}
dd if=/dev/zero of=$DIR/q6_testfile bs=1M count=256 conv=fsync 2>&1 | tail -1
sync; echo 3 > /proc/sys/vm/drop_caches 2>/dev/null || echo "(cannot drop page cache here - first read may be from RAM; run as root for a true device speed)"
echo "sequential read:"; dd if=$DIR/q6_testfile of=/dev/null bs=1M 2>&1 | tail -1
rm -f $DIR/q6_testfile
echo "rotational flag per disk (0 = SSD, 1 = HDD):"; for d in /sys/block/*/queue/rotational; do echo "$d: $(cat $d)"; done 2>/dev/null
