#!/bin/sh
# Task 3.3: the commands that would create the chosen file systems. They are only PRINTED
# (never executed) so that no real disk can be damaged by running this script.
echo "Pen drive (Windows + Linux, files > 4 GiB):  sudo mkfs.exfat -n PENDRIVE /dev/sdX1"
echo "Linux database server (journal, XFS):         sudo mkfs.xfs -L dbdata /dev/sdX1"
echo "   (or ext4 ordered):                         sudo mkfs.ext4 -L dbdata -O has_journal /dev/sdX1 ; mount -o data=ordered"
echo "Laptop (Linux):                               sudo mkfs.ext4 -L root /dev/sdX2"
echo "Laptop (Windows) / (macOS):                   NTFS (format from Windows)  /  APFS (Disk Utility)"
echo "Backup server with snapshots (checksums, CoW): sudo mkfs.btrfs -L backup /dev/sdX1 ; btrfs subvolume snapshot -r /backup /backup/.snap-\$(date +%F)"
echo "   (or ZFS):                                  sudo zpool create backup /dev/sdX && zfs snapshot backup@\$(date +%F)"
