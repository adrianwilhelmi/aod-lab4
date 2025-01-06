import subprocess
import numpy as np
import matplotlib.pyplot as plt

k_values = range(3,11)
repetitions = 100

results = []

for k in k_values:
    print("k=",k)
    for i in range(1, k+1):
        match_counts = []
        times = []
        for _ in range(repetitions):
            process = subprocess.run(
                    ["./z2", "--size", str(k), "--degree", str(i)],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True
            )

            output_lines = process.stdout.strip().split("\n")
            if len(output_lines) >= 2:
                times.append(float(output_lines[0]))
                match_counts.append(int(output_lines[1]))

        avg_match = np.mean(match_counts)
        avg_time = np.mean(times)
        results.append((k, i, avg_match, avg_time))

with open("results.csv", "w") as file:
    file.write("k,i,avg_match,avg_time\n")
    for k, i, avg_match, avg_time in results:
        file.write(f"{k},{i},{avg_match},{avg_time}\n")
