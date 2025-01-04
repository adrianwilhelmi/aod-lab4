import subprocess
import statistics
import os
import re

def run_experiment(program_path, k, repetitions):
    max_flows = []
    times = []
    aug_paths_counts = []

    for _ in range(repetitions):
        process = subprocess.Popen(
                [program_path, "--size", str(k)],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
        stdout, stderr = process.communicate()

        try:
            max_flow = int(stdout.strip())
            max_flows.append(max_flow)
        except ValueError:
            print(f"err parsing max_flow: {stdout.strip()}")

        try:
            time_str, paths_count_str = stderr.strip().split("\t")
            time = float(time_str)
            aug_paths_count = int(paths_count_str)

            times.append(time)
            aug_paths_counts.append(aug_paths_count)
        except ValueError:
            print(f"err parsing stderr: {stderr.strip()}")

    mean_max_flow = statistics.mean(max_flows) if max_flows else 0
    mean_time = statistics.mean(times) if times else 0
    mean_aug_paths_count = statistics.mean(aug_paths_counts) if aug_paths_counts else 0

    return mean_max_flow, mean_time, mean_aug_paths_count

def main():
    program_path = "./z1"
    repetitions = 50
    output_file = "results.tsv"

    if not os.path.exists(program_path):
        print(f"program {program_path} nie istnieje")
        return

    with open(output_file, "w") as file:
        file.write("k\tmean_max_flow\tmean_time\tmean_aug_paths_count\n")

        for k in range(1, 17):
            print(f"k = {k}")
            mean_max_flow, mean_time, mean_aug_paths_count = run_experiment(
                    program_path, k, repetitions)

            file.write(f"{k}\t{mean_max_flow:.3f}\t{mean_time:.6f}\t{mean_aug_paths_count:.3f}\n")

if __name__ == "__main__":
    main()
