#!/usr/bin/env python3
import json
import sys

RESET = "\033[0m"
BOLD = "\033[1m"

def color_for_diff(diff):
    if diff <= -20:
        return "\033[92m"
    elif diff <= -5:
        return "\033[32m"
    elif diff < 5:
        return "\033[33m"
    elif diff < 20:
        return "\033[31m"
    else:
        return "\033[91m"

def main():
    if len(sys.argv) < 2:
        print("Usage: compare_vectors.py <benchmark.json>")
        sys.exit(1)

    with open(sys.argv[1]) as f:
        data = json.load(f)

    benchmarks = data["benchmarks"]
    ds_results = {}
    std_results = {}

    for b in benchmarks:
        name = b["name"]
        time = b["real_time"]
        if name.startswith("BM_DsVector_"):
            ds_results[name.replace("BM_DsVector_", "")] = time
        elif name.startswith("BM_StdVector_"):
            std_results[name.replace("BM_StdVector_", "")] = time

    print(f"{BOLD}{'Benchmark':<30} {'ds::Vector':>12} {'std::vector':>12} {'Diff':>10}{RESET}")
    print("-" * 66)

    for key in sorted(ds_results.keys()):
        if key in std_results:
            ds_time = ds_results[key]
            std_time = std_results[key]
            diff = ((ds_time - std_time) / std_time) * 100
            color = color_for_diff(diff)
            print(f"{key:<30} {ds_time:>10.1f}ns {std_time:>10.1f}ns {color}{diff:>+7.1f}%{RESET}")

if __name__ == "__main__":
    main()
