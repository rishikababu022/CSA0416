#!/usr/bin/env python3
"""Plots for Q4 (faults vs frames) and Q5 (faults vs frames per workload) from the CSV files."""
import csv, sys
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

def read(path):
    with open(path) as f:
        return list(csv.DictReader(f))

try:
    rows = read("Q4_faults_vs_frames.csv")
    x = [int(r["frames"]) for r in rows]
    plt.figure(figsize=(6, 4))
    for k in ("FIFO", "LRU", "Optimal"):
        plt.plot(x, [int(r[k]) for r in rows], marker="o", label=k)
    plt.xlabel("number of frames"); plt.ylabel("page faults"); plt.title("Q4: faults vs frames (20-reference string)")
    plt.grid(alpha=.3); plt.legend(); plt.tight_layout(); plt.savefig("Q4_faults_vs_frames.png", dpi=130); plt.close()
    print("wrote Q4_faults_vs_frames.png")
except FileNotFoundError:
    print("Q4_faults_vs_frames.csv missing")

try:
    rows = read("Q5_faults.csv")
    for w in sorted({r["workload"] for r in rows}):
        sub = [r for r in rows if r["workload"] == w]
        x = [int(r["frames"]) for r in sub]
        plt.figure(figsize=(6, 4))
        for k in ("FIFO", "LRU", "Optimal", "Clock"):
            plt.plot(x, [int(r[k]) for r in sub], marker="o", label=k)
        plt.xlabel("number of frames"); plt.ylabel("page faults (100,000 references)")
        plt.title("Q5: %s workload" % w); plt.grid(alpha=.3); plt.legend(); plt.tight_layout()
        plt.savefig("Q5_faults_%s.png" % w, dpi=130); plt.close()
        print("wrote Q5_faults_%s.png" % w)
except FileNotFoundError:
    print("Q5_faults.csv missing")
