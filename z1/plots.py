import matplotlib.pyplot as plt
import pandas as pd

def plot_results(filename):
    data = pd.read_csv(filename, sep="\t")

    plt.figure(figsize=(10,6))
    plt.plot(data['k'], data['mean_max_flow'], marker='o', color='b', label="Średnia maksymalna wartość przepływu")
    plt.xlabel('k')
    plt.ylabel('Maksymalna wartość przepływu [-]')
    plt.title('Wartość maksymalnego przepływu w zależności od wymiaru hiperkostki')
    plt.legend()
    plt.savefig("mean_max_flow.pdf")
    plt.show()

    plt.figure(figsize=(10,6))
    plt.plot(data['k'], data['mean_time'], marker='o', color='b', label="Średni czas działania")
    plt.xlabel('k')
    plt.ylabel('Czas [s]')
    plt.title("Średni czas działania programu w zależności od wymiaru hiperkostki")
    plt.legend()
    plt.savefig("mean_time.pdf")
    plt.show()

    plt.figure(figsize=(10,6))
    plt.plot(data['k'], data['mean_aug_paths_count'], marker='o', color='b', label="Średnia ilość ścieżek powiększających")
    plt.xlabel('k')
    plt.ylabel('Ilość ścieżek powiększających [-]')
    plt.title("Średnia ilość ścieżek rozszerzających w zależności od wymiaru hiperkostki")
    plt.legend()
    plt.savefig("mean_aug_paths.pdf")
    plt.show()

plot_results('results.tsv')



