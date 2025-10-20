#!/usr/bin/env python3

import os
import subprocess
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline

PROJECT_ROOT = os.path.dirname(os.path.abspath(__file__))
STATS_DIR = os.path.join(PROJECT_ROOT, "statistics")
TESTS_DIR = os.path.join(PROJECT_ROOT, "tests")
TREE_OUT_DIR = os.path.join(STATS_DIR, "tree-time-results")
SET_OUT_DIR = os.path.join(STATS_DIR, "set-time-results")
TIME_FILE = os.path.join(STATS_DIR, "time_comparison.txt")

BUILD_DIR = os.path.join(PROJECT_ROOT, "build")

os.makedirs(TREE_OUT_DIR, exist_ok=True)
os.makedirs(SET_OUT_DIR, exist_ok=True)

TESTS_NUM = 8

# Размеры тестов
SIZES = np.array([1_000, 10_000, 25_000, 50_000, 100_000, 250_000, 500_000, 1_000_000])
test_files = [f"test{i}.dat" for i in range(1, TESTS_NUM + 1)]

def run_benchmark(exe_name, output_dir):
    print(f"\nRunning benchmarks for {exe_name}:")
    for test in test_files:
        test_path = os.path.join(TESTS_DIR, test)
        out_path = os.path.join(output_dir, test.replace(".dat", ".txt"))
        exe_path = os.path.join(BUILD_DIR, exe_name)
        
        with open(test_path, 'r') as fin, open(out_path, 'w') as fout:
            subprocess.run([exe_path], stdin=fin, stdout=fout, stderr=subprocess.STDOUT, text=True)
        print(f"  {test} --> {out_path}")

def extract_time(output_file):
    """Извлекает время из файла вида '... Time: X.XXX s'"""
    try:
        with open(output_file, 'r') as f:
            for line in f:
                if "Time:" in line:
                    parts = line.split()
                    for i, part in enumerate(parts):
                        if part == "Time:":
                            return float(parts[i+1])
    except Exception as e:
        print(f"Error reading {output_file}: {e}")
        return 0.0
      
    return 0.0

def collect_results():
    print("\nCollecting results into time_comparison.txt...")
    tree_times = []
    set_times = []

    for i in range(1, TESTS_NUM + 1):
        rb_t = extract_time(os.path.join(TREE_OUT_DIR, f"test{i}.txt"))
        set_t = extract_time(os.path.join(SET_OUT_DIR, f"test{i}.txt"))
        tree_times.append(rb_t)
        set_times.append(set_t)

    # Сохраняем time.txt
    with open(TIME_FILE, 'w') as f:
        f.write("==========RB_TREE==========\n")
        for i, t in enumerate(tree_times, 1):
            f.write(f"test{i}: {t:.3f} s\n")
        f.write("\n============SET============\n")
        for i, t in enumerate(set_times, 1):
            f.write(f"test{i}: {t:.3f} s\n")

    print(f"\nResults saved to {TIME_FILE}")
    return np.array(tree_times), np.array(set_times)

def plot_results(tree_times, set_times):
    print("\nGenerating plots...")
    sizes_smooth = np.linspace(SIZES.min(), SIZES.max(), 300)

    spl_rb = make_interp_spline(SIZES, tree_times, k=2)
    spl_set = make_interp_spline(SIZES, set_times, k=2)
    rb_smooth = np.maximum(spl_rb(sizes_smooth), 0)
    set_smooth = np.maximum(spl_set(sizes_smooth), 0)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))

    # === График 1: Логарифмическая шкала ===
    ax1.plot(sizes_smooth, rb_smooth, '-', color='tab:blue', linewidth=2.5, label='RB-Tree (O(log n) distance)')
    ax1.plot(sizes_smooth, set_smooth, '-', color='tab:red', linewidth=2.5, label='std::set (O(k) distance)')
    ax1.plot(SIZES, tree_times, 'o', color='tab:blue', markersize=6)
    ax1.plot(SIZES, set_times, 's', color='tab:red', markersize=6)
    ax1.set_yscale('log')
    ax1.set_xlabel('Количество операций')
    ax1.set_ylabel('Время, с')
    ax1.set_title('Сравнение производительности: RB-Tree vs std::set (логарифмическая шкала)')
    ax1.legend()
    ax1.grid(True, which="both", ls="--", linewidth=0.5)

    # === График 2: Линейная шкала ===
    ax2.plot(sizes_smooth, rb_smooth, '-', color='tab:blue', linewidth=2.5, label='RB-Tree (O(log n) distance)')
    ax2.plot(sizes_smooth, set_smooth, '-', color='tab:red', linewidth=2.5, label='std::set (O(k) distance)')
    ax2.plot(SIZES, tree_times, 'o', color='tab:blue', markersize=6)
    ax2.plot(SIZES, set_times, 's', color='tab:red', markersize=6)
    ax2.set_xlabel('Количество операций')
    ax2.set_ylabel('Время, с')
    ax2.set_title('Сравнение производительности: RB-Tree vs std::set (линейная шкала)')
    ax2.legend()
    ax2.grid(True, ls="--", linewidth=0.5)

    # Аннотации
    ax2.annotate(f'{tree_times[-1]:.3f} с', 
                 xy=(SIZES[-1], tree_times[-1]), 
                 xytext=(10, 10), 
                 textcoords='offset points',
                 fontsize=10,
                 bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.7))
    ax2.annotate(f'{int(set_times[-1])} с', 
                 xy=(SIZES[-1], set_times[-1]), 
                 xytext=(10, -15), 
                 textcoords='offset points',
                 fontsize=10,
                 bbox=dict(boxstyle="round,pad=0.3", facecolor="lightcoral", alpha=0.7))

    plt.tight_layout()
    plot_path = os.path.join(STATS_DIR, "tree_vs_set_performance.png")
    plt.savefig(plot_path, dpi=300, bbox_inches='tight')
    print(f"Plot saved to {plot_path}")
    plt.close()

if __name__ == "__main__":
    tree_exe = "tree_bench"
    set_exe = "set_bench"

    run_benchmark(tree_exe, TREE_OUT_DIR)
    run_benchmark(set_exe, SET_OUT_DIR)

    tree_times, set_times = collect_results()

    plot_results(tree_times, set_times)
    