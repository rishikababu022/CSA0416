#!/bin/sh
# Task 3.4: inspect a real file system (inodes, block size, free space).
# Works without root: it creates a small ext4 image file and inspects the image and the
# file system that holds the current directory.
echo "### System"; uname -a; [ -r /etc/os-release ] && . /etc/os-release && echo "$PRETTY_NAME"
echo; echo "### df -T -h . (file system holding this directory)"; df -T -h .
echo; echo "### df -i . (inode usage)"; df -i .
echo; echo "### ls -li (inode numbers)"; ls -li | head -n 8
echo; echo "### stat on this script"; stat "$0"
echo; echo "### stat -f . (file-system level: block size, blocks, inodes)"; stat -f .

IMG=Task03_demo.img
if command -v mkfs.ext4 >/dev/null 2>&1 || command -v mke2fs >/dev/null 2>&1; then
    dd if=/dev/zero of=$IMG bs=1M count=64 2>/dev/null
    if command -v mkfs.ext4 >/dev/null 2>&1; then mkfs.ext4 -F -q $IMG; else mke2fs -t ext4 -F -q $IMG; fi
    echo; echo "### dumpe2fs -h $IMG"
    dumpe2fs -h $IMG 2>&1 | grep -E "Filesystem features|Block size|Block count|Free blocks|Inode count|Free inodes|Inode size|Journal|Default mount options"
    if command -v tune2fs >/dev/null 2>&1; then
        echo; echo "### tune2fs -l $IMG (first lines)"; tune2fs -l $IMG 2>&1 | head -n 25
    fi
    rm -f $IMG
else
    echo; echo "mkfs.ext4 / mke2fs not installed: install e2fsprogs and rerun to see dumpe2fs / tune2fs output."
fi
