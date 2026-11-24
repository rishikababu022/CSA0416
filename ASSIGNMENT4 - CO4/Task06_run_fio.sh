#!/bin/sh
# Runs the two fio jobs if fio is installed. Usage: HDD_DIR=/mnt/hdd SSD_DIR=/mnt/ssd sh Task06_run_fio.sh
if ! command -v fio >/dev/null 2>&1; then
    echo "fio is not installed (sudo apt install fio); the C simulation Task06_sim.c was used instead."
    exit 0
fi
DIR="${HDD_DIR:-/tmp}" fio Task06_designA_hdd.fio
DIR="${SSD_DIR:-/tmp}" fio Task06_designB_ssd.fio
