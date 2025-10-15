import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline

# Данные
sizes = np.array([1_000, 10_000, 25_000, 50_000, 100_000, 250_000, 500_000, 1_000_000])
rb_tree_times = np.array([0.002, 0.02, 0.05, 0.076, 0.147, 0.375, 0.772, 1.634])
set_times = np.array([0.002, 0.043, 0.21, 1.082, 5.309, 39.862, 182.375, 1017.316])

sizes_smooth = np.linspace(sizes.min(), sizes.max(), 300)

spl_rb = make_interp_spline(sizes, rb_tree_times, k=2)
spl_set = make_interp_spline(sizes, set_times, k=2)

rb_smooth = spl_rb(sizes_smooth)
set_smooth = spl_set(sizes_smooth)

rb_smooth = np.maximum(rb_smooth, 0)
set_smooth = np.maximum(set_smooth, 0)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))

# === График 1: Логарифмическая шкала ===
ax1.plot(sizes_smooth, rb_smooth, '-', color='tab:blue', linewidth=2.5, label='RB-Tree (O(log n) distance)')
ax1.plot(sizes_smooth, set_smooth, '-', color='tab:red', linewidth=2.5, label='std::set (O(k) distance)')
ax1.plot(sizes, rb_tree_times, 'o', color='tab:blue', markersize=6)
ax1.plot(sizes, set_times, 's', color='tab:red', markersize=6)

ax1.set_yscale('log')
ax1.set_xlabel('Количество элементов')
ax1.set_ylabel('Время, с')
ax1.set_title('Сравнение производительности: RB-Tree vs std::set (логарифмическая шкала)')
ax1.legend()
ax1.grid(True, which="both", ls="--", linewidth=0.5)

# === График 2: Линейная шкала ===
ax2.plot(sizes_smooth, rb_smooth, '-', color='tab:blue', linewidth=2.5, label='RB-Tree (O(log n) distance)')
ax2.plot(sizes_smooth, set_smooth, '-', color='tab:red', linewidth=2.5, label='std::set (O(k) distance)')
ax2.plot(sizes, rb_tree_times, 'o', color='tab:blue', markersize=6)
ax2.plot(sizes, set_times, 's', color='tab:red', markersize=6)

ax2.set_xlabel('Количество элементов')
ax2.set_ylabel('Время, с')
ax2.set_title('Сравнение производительности: RB-Tree vs std::set (линейная шкала)')
ax2.legend()
ax2.grid(True, ls="--", linewidth=0.5)

# Аннотации на линейном графике
ax2.annotate(f'{rb_tree_times[-1]:.3f} с', 
             xy=(sizes[-1], rb_tree_times[-1]), 
             xytext=(10, 10), 
             textcoords='offset points',
             fontsize=10,
             bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.7))

ax2.annotate(f'{int(set_times[-1])} с', 
             xy=(sizes[-1], set_times[-1]), 
             xytext=(10, -15), 
             textcoords='offset points',
             fontsize=10,
             bbox=dict(boxstyle="round,pad=0.3", facecolor="lightcoral", alpha=0.7))

plt.tight_layout()
plt.savefig('rb_tree_vs_set_performance_smooth.png', dpi=300, bbox_inches='tight')
plt.show()