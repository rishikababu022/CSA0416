#!/usr/bin/env python3
"""Small HTTP load generator (fallback for ab / wrk / hey, which are not installed everywhere).

  python3 B3_loadtest.py http://localhost:8000/result/ABC123 --concurrency 8 --duration 10
  python3 B3_loadtest.py http://localhost:8000/health --wait        # seconds until the service answers 200

Each thread keeps one connection open. The client is Python, so on a small machine the load generator
itself can be the bottleneck; compare with ab/wrk when they are available:  ab -n 20000 -c 32 URL
"""
import argparse
import http.client
import statistics
import threading
import time
from urllib.parse import urlparse


def wait_ready(url, timeout=60):
    u = urlparse(url)
    t0 = time.time()
    while time.time() - t0 < timeout:
        try:
            c = http.client.HTTPConnection(u.hostname, u.port or 80, timeout=1)
            c.request("GET", u.path or "/")
            if c.getresponse().status == 200:
                return time.time() - t0
        except OSError:
            pass
        time.sleep(0.02)
    return None


def worker(u, stop, lat, errors):
    conn = http.client.HTTPConnection(u.hostname, u.port or 80, timeout=10)
    while time.time() < stop:
        t = time.perf_counter()
        try:
            conn.request("GET", u.path or "/")
            r = conn.getresponse()
            r.read()
            if r.status != 200:
                errors.append(r.status)
            lat.append(time.perf_counter() - t)
        except OSError:
            errors.append("io")
            conn = http.client.HTTPConnection(u.hostname, u.port or 80, timeout=10)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("url")
    ap.add_argument("--concurrency", type=int, default=8)
    ap.add_argument("--duration", type=float, default=10)
    ap.add_argument("--wait", action="store_true", help="only measure time until the first HTTP 200")
    a = ap.parse_args()
    if a.wait:
        s = wait_ready(a.url)
        print("not ready within 60 s" if s is None else f"ready after {s:.2f} s")
        return
    u = urlparse(a.url)
    lat, errors, threads = [], [], []
    stop = time.time() + a.duration
    for _ in range(a.concurrency):
        t = threading.Thread(target=worker, args=(u, stop, lat, errors))
        t.start()
        threads.append(t)
    for t in threads:
        t.join()
    n = len(lat)
    lat.sort()
    print(f"url={a.url} concurrency={a.concurrency} duration={a.duration}s")
    print(f"requests={n} errors={len(errors)} requests_per_second={n / a.duration:.1f}")
    if n:
        print(f"latency_ms mean={statistics.mean(lat)*1000:.1f} p50={lat[n//2]*1000:.1f} "
              f"p95={lat[int(n*0.95)]*1000:.1f} p99={lat[int(n*0.99)]*1000:.1f} max={lat[-1]*1000:.1f}")


if __name__ == "__main__":
    main()
