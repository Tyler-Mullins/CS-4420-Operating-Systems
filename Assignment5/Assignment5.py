# Import necessary modules
import sys
from dataclasses import dataclass

# I created a structure for the processes, that way I can use this to
# easily keep track of the different data fields of each process
@dataclass
class Process:

    # Given data
    pid: int
    arrival_time: int
    burst_time: int

    # New data that I needed
    remaining_time: int = 0
    start_time: int = -1
    end_time: int = -1
    wait_time: int = 0
    remaining: int = 0
    is_complete: bool = False

    def __post_init__(self):
        self.remaining_time = self.burst_time

# First Come, First Serve (FCFS) Algorithm
# -----------------------------------------
# The first process the show up is the first
# one that gets computed to its full
def FCFS(input_file):

    # Variables necessary for the algorithm
    # *** THESE WILL BE REUSED FOR THE REMAINDER OF
    # THE PROGRAM ***
    total_time = 0
    total_wait = 0
    avg_time = 0
    processes = []

    # Open a file with read permissions
    with open(input_file, "r") as file:

        # The first line in the file should be the number of
        # processes
        num_processes = int(file.readline())

        # Iterating through the list using the number of processes
        for i in range(num_processes):

            # Putting id, arrival time, and burst time into a Process object
            id, atime, btime = map(int, file.readline().split())

            # Creating a Process object with the given data
            proc = Process (pid = id, arrival_time = atime, burst_time = btime)

            # Adding these processes to an array
            processes.append(proc)

        # Using an enumerated for loop to iterate through the list
        for i, proc in enumerate(processes):

            # Output to notify that it started
            print("Starting Process ", proc.pid, ": Starting time: ", total_time, "\n")

            # Initializing the process to the current total time
            proc.start_time = total_time

            # Updating the total time and process end time
            total_time += proc.burst_time
            proc.end_time = total_time

            # Calculating the wait time
            proc.wait_time = proc.start_time - proc.arrival_time
            total_wait += proc.wait_time

            # Marking the process as complete
            proc.is_complete = True
            print("Process ", proc.pid, " has finished\n")

            # Checking to see if the next process should start immediately or if there is idle time
            if i + 1 < len(processes):
                processes[i + 1].start_time = total_time

                # If the next process has not arrived yet, there will be idle time
                if processes[i + 1].arrival_time > total_time:
                    print("Idle")
                    processes[i + 1].start_time = processes[i + 1].arrival_time

    # Calculating and printing the average wait time
    avg_time = total_wait / num_processes
    print ("Average wait time: ", avg_time)

# Shortest Job First (SJF) Algorithm
# -----------------------------------------
# The shortest burst time is evaluated first
def SJF(input_file):

    # Variables necessary for the algorithm
        # *** THESE WILL BE REUSED FOR THE REMAINDER OF
        # THE PROGRAM ***
    num_complete = 0
    total_time = 0
    total_wait = 0
    avg_time = 0
    processes = []

    # Open a file with read permission
    with open(input_file, "r") as file:

        # Getting the number of processes
        num_processes = int(file.readline())

        # Creating processes with the information given (This information is the same amongst
        # all three algorithms)
        for i in range(num_processes):
            id, atime, btime = map(int, file.readline().split())

            proc = Process (pid = id, arrival_time = atime, burst_time = btime)

            # Adding the processes to an array
            processes.append(proc)

        # Ensuring we stay within the array
        while num_complete < num_processes:

            # Initializing  variables to help find the smallest process
            smallest = 0
            smallest_process = -1

            # Iterating through the array to find the process with the smallest burst time that has arrived
            for i, proc in enumerate(processes):

                # Making sure the process is not complete
                if not proc.is_complete and proc.arrival_time <= total_time:

                    # Checking if this process has a smaller burst time than the current smallest
                    if proc.burst_time < smallest or smallest == 0:

                        # If so, update smallest
                        smallest = proc.burst_time
                        smallest_process = i

            # Selecting the smallest process to run next
            proc = processes[smallest_process]

            # Outputting the start of the process
            print("Starting Process ", proc.pid, ": Starting time: ", total_time, "\n")

            # Making the process' start time equal to the total time so far
            proc.start_time = total_time
            total_time += proc.burst_time
            proc.end_time = total_time

            # Calculating the wait time for the process
            proc.wait_time = proc.start_time - proc.arrival_time
            total_wait += proc.wait_time

            # Finishing the process
            proc.is_complete = True
            num_complete += 1
            print("Process ", proc.pid, " has finished\n")

    # Calculating and outputting the average wait time
    avg_time = total_wait / num_processes
    print ("Average wait time: ", avg_time)

# Round Robin (RR) Algorithm
# -----------------------------------------
# Each process is given a fixed time quantum
# to process
def RR(input_file, quantum):

    # Variables necessary for the algorithm
    # *** THESE WILL BE REUSED FOR THE REMAINDER OF
    # THE PROGRAM ***
    num_complete = 0
    total_time = 0
    total_wait = 0
    avg_time = 0
    processes = []

    # Opening a file with read permissions
    with open(input_file, "r") as file:

        # Getting the number of processes
        num_processes = int(file.readline())

        # Iterating through the processes and creating Process objects
        for i in range(num_processes):
            id, atime, btime = map(int, file.readline().split())

            proc = Process (pid = id, arrival_time = atime, burst_time = btime, remaining = btime)

            # Adding these new processes to an array
            processes.append(proc)

        # Starting the Round Robin scheduling loop
        while num_complete < num_processes:
            for i, proc in enumerate(processes):

                # Making sure we are only grabbing incomplete processes, processes that have actually arrived, and
                # that we still have processes to grab
                if proc.is_complete == False and proc.remaining > 0 and proc.arrival_time <= total_time:

                    # Outputting the start of the process
                    print("Starting Process ", proc.pid, ": Starting time: ", total_time, "\n")

                    # If the process is larger than or the same as the quantum...
                    if proc.remaining >= quantum:

                        # Simply subtract the quantum from the remaining time 
                        # and adjust the total time accordingly
                        proc.remaining -= quantum
                        total_time += quantum

                    # If the process is less than the quantum...
                    elif proc.remaining < quantum and proc.remaining > 0:

                        # Add the remaining time to the total time and THEN
                        # remove the remaining time
                        total_time += proc.remaining
                        proc.remaining -= proc.remaining

                    # If the process has no more time remaining...
                    if proc.remaining == 0:

                        # It is complete
                        proc.is_complete = True

                        # Calculate the wait time
                        proc.end_time = total_time
                        proc.wait_time = (proc.end_time - proc.arrival_time - proc.burst_time)
                        total_wait += proc.wait_time

                        # Output that it is finished and update the number of completed processes
                        print("Process ", proc.pid, " has finished\n")
                        num_complete += 1

    # Calculate the average wait time
    avg_time = total_wait / num_processes
    print ("Average wait time: ", avg_time)


def main():

    # If the user did not put in the required amount of objects, tell them
    if len(sys.argv) < 3:
        print("Usage: python proj2.py input_file [FCFS|RR|SJF] [time_quantum]")
        return

    # Using argv(), which I did not know was in Python, to extract important pieces
    input_file = sys.argv[1]
    algorithm = sys.argv[2]

    # Determine which algorithm to use
    if algorithm == "FCFS":
        FCFS(input_file)

    elif algorithm == "SJF":
        SJF(input_file)

    elif algorithm == "RR":

        # Check to see if user included a time quantum
        if len(sys.argv) < 4:
            print("Round Robin requires a time quantum.")
            return

        quantum = int(sys.argv[3])

        RR(input_file, quantum)

    # If it is anything else, the user is wrong
    else:
        print("Invalid scheduling algorithm.")
        print("Use FCFS, SJF, or RR.")


if __name__ == "__main__":
    main()
