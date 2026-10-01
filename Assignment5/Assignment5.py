import sys
from dataclasses import dataclass

@dataclass
class Process:
    pid: int
    arrival_time: int
    burst_time: int

    remaining_time: int = 0
    start_time: int = -1
    end_time: int = -1
    wait_time: int = 0
    remaining: int = 0
    is_complete: bool = False

    def __post_init__(self):
        self.remaining_time = self.burst_time

def FCFS(input_file):

    total_time = 0
    total_wait = 0
    avg_time = 0
    processes = []

    with open(input_file, "r") as file:
        num_processes = int(file.readline())

        for i in range(num_processes):
            id, atime, btime = map(int, file.readline().split())

            proc = Process (pid = id, arrival_time = atime, burst_time = btime)

            processes.append(proc)

        for i, proc in enumerate(processes):
            print("Starting Process " proc.pid ": Starting time: " total_time "\n")

            total_time += proc.burst_time
            proc.end_time = total_time
            proc.wait_time = proc.start_time - proc.arrival_time
            total_wait += proc.wait_time
            proc.is_complete = True
            print("Process " proc.pid " has finished\n")

            if i + 1 < len(processes):
                processes[i + 1].start_time = total_time
                if processes[i + 1].arrival_time > total_time:
                    print("Idle")
                    processes[i + 1].start_time = processes[i + 1].arrival_time

    avg_time = total_wait / num_processes
    print ("Average wait time: " avg_time)

def SJF(input_file):
    num_complete = 0
    total_time = 0
    total_wait = 0
    avg_time = 0
    processes = []

    with open(input_file, "r") as file:
        num_processes = int(file.readline())

        for i in range(num_processes):
            id, atime, btime = map(int, file.readline().split())

            proc = Process (pid = id, arrival_time = atime, burst_time = btime)

            processes.append(proc)
        while num_complete < num_processes:
            smallest = 0
            smallest_process = -1
            for i, proc in enumerate(processes):
                if not proc.is_complete and proc.arrival_time <= total_time:
                    if proc.burst_time < smallest or smallest == 0:
                        smallest = proc.burst_time
                        smallest_process = i
            proc = processes[smallest_process]
            print(f"Starting Process {proc.pid}: Starting time: {total_time}\n")
            proc.start_time = total_time
            total_time += proc.burst_time
            proc.end_time = total_time
            proc.wait_time = proc.start_time - proc.arrival_time
            total_wait += proc.wait_time
            proc.is_complete = True
            num_complete += 1
            print(f"Process {proc.pid} has finished\n")

    avg_time = total_wait / num_processes
    print ("Average wait time: " avg_time)

def RR(input_file, quantum):
    num_complete = 0
    total_time = 0
    total_wait = 0
    avg_time = 0
    processes = []

    with open(input_file, "r") as file:
        num_processes = int(file.readline())

        for i in range(num_processes):
            id, atime, btime = map(int, file.readline().split())

            proc = Process (pid = id, arrival_time = atime, burst_time = btime, remaining = btime)

            processes.append(proc)

        while num_complete < num_processes:
            for i, proc in enumerate(processes):
                if proc.is_complete == False and proc.remaining > 0 and proc.arrival_time <= total_time:
                    print("Starting Process " + str(proc.pid) + ": Starting time: " + str(total_time) + "\n")
                    if proc.remaining >= quantum:
                        proc.remaining -= quantum
                        total_time += quantum
                    elif proc.remaining < quantum and proc.remaining > 0:
                        total_time += proc.remaining
                        proc.remaining -= proc.remaining
                    if proc.remaining == 0:
                        proc.is_complete = True
                        proc.end_time = total_time
                        proc.wait_time = (proc.end_time - proc.arrival_time - proc.burst_time)
                        total_wait += proc.wait_time
                        print(f"Process {proc.pid} has finished\n")
                        num_complete += 1
                
    avg_time = total_wait / num_processes
    print ("Average wait time: " avg_time)


def main():

    if len(sys.argv) < 3:
        print("Usage: python proj2.py input_file [FCFS|RR|SJF] [time_quantum]")
        return

    input_file = sys.argv[1]
    algorithm = sys.argv[2]

    if algorithm == "FCFS":
        FCFS(input_file)

    elif algorithm == "SJF":
        SJF(input_file)

    elif algorithm == "RR":

        if len(sys.argv) < 4:
            print("Round Robin requires a time quantum.")
            return

        quantum = int(sys.argv[3])

        RR(input_file, quantum)

    else:
        print("Invalid scheduling algorithm.")
        print("Use FCFS, SJF, or RR.")


if __name__ == "__main__":
    main()