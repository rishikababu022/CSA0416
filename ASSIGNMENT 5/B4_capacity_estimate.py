#!/usr/bin/env python3
"""B4.2 - containers / VMs needed for 150,000 students in two hours.

  python3 B4_capacity_estimate.py --rps-container 450 --rps-vm 900

--rps-* are YOUR measured requests per second for one container (with its CPU limit) and one VM.
The other inputs are assumptions you can change and must justify in your report.
"""
import argparse
import math

ap = argparse.ArgumentParser()
ap.add_argument("--students", type=int, default=150000)
ap.add_argument("--window-hours", type=float, default=2.0)
ap.add_argument("--requests-per-student", type=float, default=8, help="assumption: login + result + assets")
ap.add_argument("--peak-factor", type=float, default=3.0, help="assumption: busiest minute vs the 2-hour average")
ap.add_argument("--target-utilisation", type=float, default=0.7, help="keep 30%% head-room")
ap.add_argument("--rps-container", type=float, required=True)
ap.add_argument("--rps-vm", type=float, required=True)
a = ap.parse_args()

total = a.students * a.requests_per_student
avg_rps = total / (a.window_hours * 3600)
peak_rps = avg_rps * a.peak_factor
print(f"total requests            : {total:,.0f}")
print(f"average rate              : {avg_rps:,.0f} requests/s")
print(f"design (peak) rate        : {peak_rps:,.0f} requests/s  (peak factor {a.peak_factor})")
for name, r in (("container", a.rps_container), ("virtual machine", a.rps_vm)):
    n = math.ceil(peak_rps / (r * a.target_utilisation))
    print(f"{name:16s}: measured {r:,.0f} req/s each -> {n} needed at {a.target_utilisation:.0%} utilisation "
          f"(+1 spare = {n + 1})")
