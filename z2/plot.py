import pandas as pd
import matplotlib.pyplot as plt

results = pd.read_csv("results.csv")

for k in results['k'].unique():
    subset = results[results['k'] == k]
    plt.figure(figsize=(8,6))
    plt.plot(subset['i'], subset['avg_match'], marker='o', label=f"k={k}")
    plt.title(f"Wielkość maksymalnego skojarzenia w zależności od i dla k={k}")
    plt.xlabel("stopień wierzchołków w V_1")
    plt.ylabel("Średnia wielkość maksymalnego skojarzenia")
    plt.legend()
    plt.savefig(f"plots/max_matching_k{k}.pdf")
    plt.show()

for i in results['i'].unique():
    subset = results[results['i'] == i]
    plt.figure(figsize=(8,6))
    plt.plot(subset['k'], subset['avg_time'], marker='o', label=f"i={i}")
    plt.title(f"Czas działania programu w zależności od k dla i={i}")
    plt.xlabel("k (log2 z liczby wierzchołków w V1)")
    plt.ylabel("Średni czas działania algorytmu [ms]")
    plt.legend()
    plt.savefig(f"plots/time_execution_i{i}.pdf")
    plt.show()

