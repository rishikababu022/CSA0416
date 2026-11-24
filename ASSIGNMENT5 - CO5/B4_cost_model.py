#!/usr/bin/env python3
"""B4.3 - yearly cost model for the result portal (one result day per semester = 2 per year).

!! Every price below is a PLACEHOLDER ASSUMPTION. Replace them with the current quotes of your
!! cloud provider and the real price of the university's servers before using the numbers.
"""
import argparse

ap = argparse.ArgumentParser()
ap.add_argument("--nodes-peak", type=int, required=True, help="nodes/VMs needed on result day (from B4_capacity_estimate.py)")
ap.add_argument("--nodes-base", type=int, default=2, help="nodes needed on a normal day")
ap.add_argument("--cloud-price-hour", type=float, default=0.10, help="ASSUMPTION: USD per node-hour")
ap.add_argument("--server-price", type=float, default=3000.0, help="ASSUMPTION: USD per own server (purchase)")
ap.add_argument("--server-years", type=float, default=4.0, help="depreciation period")
ap.add_argument("--server-run-year", type=float, default=600.0, help="ASSUMPTION: USD per server-year for power, cooling, admin share")
ap.add_argument("--result-days", type=int, default=2)
ap.add_argument("--peak-hours-per-day", type=float, default=6.0, help="ramp-up + 2 h peak + ramp-down")
a = ap.parse_args()

H = 24 * 365
peak_hours = a.result_days * a.peak_hours_per_day
cloud_fixed = a.nodes_peak * a.cloud_price_hour * H
cloud_auto = a.nodes_base * a.cloud_price_hour * (H - peak_hours) + a.nodes_peak * a.cloud_price_hour * peak_hours
own = a.nodes_peak * (a.server_price / a.server_years + a.server_run_year)
print(f"Own servers sized for result day ({a.nodes_peak} servers)       : {own:>12,.0f} USD/year")
print(f"Cloud, fixed VMs sized for result day, 24x7              : {cloud_fixed:>12,.0f} USD/year")
print(f"Cloud, autoscaled ({a.nodes_base} base, {a.nodes_peak} for {peak_hours:.0f} h/year) : {cloud_auto:>12,.0f} USD/year")
hybrid = a.nodes_base * (a.server_price / a.server_years + a.server_run_year) + (a.nodes_peak - a.nodes_base) * a.cloud_price_hour * peak_hours
print(f"Hybrid (own base servers, burst to cloud on result day)  : {hybrid:>12,.0f} USD/year")
print(f"Idle share of fixed capacity: {100 * (1 - (a.nodes_base * (H - peak_hours) + a.nodes_peak * peak_hours) / (a.nodes_peak * H)):.1f}% of node-hours are not needed")
