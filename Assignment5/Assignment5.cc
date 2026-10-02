// Import necessary modules

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// I created a structure for the processes, that way I can use this to
// easily keep track of the different data fields of each process

struct Process
{
    // Given data
    int pid;
    int arrival_time;
    int burst_time;

    // New data that I needed
    int remaining_time = 0;
    int start_time = -1;
    int end_time = -1;
    int wait_time = 0;
    int remaining = 0;
    bool is_complete = false;

    Process(int pid, int arrival_time, int burst_time, int remaining = 0)
    {
        this->pid = pid;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        this->remaining = remaining;

        // Equivalent to __post_init__
        this->remaining_time = this->burst_time;
    }
};


// First Come, First Serve (FCFS) Algorithm
// -----------------------------------------
// The first process to show up is the first
// one that gets computed to its full

void FCFS(std::string input_file)
{
    // Variables necessary for the algorithm
    // *** THESE WILL BE REUSED FOR THE REMAINDER OF
    // THE PROGRAM ***

    int total_time = 0;
    int total_wait = 0;
    double avg_time = 0;
    std::vector<Process> processes;

    // Open a file with read permissions
    std::ifstream file(input_file);

    // The first line in the file should be the number of
    // processes
    int num_processes;
    file >> num_processes;

    // Iterating through the list using the number of processes
    for (int i = 0; i < num_processes; i++)
    {
        // Putting id, arrival time, and burst time into a Process object
        int id, atime, btime;
        file >> id >> atime >> btime;

        // Creating a Process object with the given data
        Process proc(id, atime, btime);

        // Adding these processes to an array
        processes.push_back(proc);
    }

    // Using an enumerated for loop to iterate through the list
    for (size_t i = 0; i < processes.size(); i++)
    {
        Process& proc = processes[i];

        // Output to notify that it started
        std::cout << "Starting Process " << proc.pid
                  << ": Starting time: " << total_time << "\n"
                  << std::endl;

        // Initializing the process to the current total time
        proc.start_time = total_time;

        // Updating the total time and process end time
        total_time += proc.burst_time;
        proc.end_time = total_time;

        // Calculating the wait time
        proc.wait_time = proc.start_time - proc.arrival_time;
        total_wait += proc.wait_time;

        // Marking the process as complete
        proc.is_complete = true;

        std::cout << "Process " << proc.pid << " has finished\n"
                  << std::endl;

        // Checking to see if the next process should start immediately
        // or if there is idle time
        if (i + 1 < processes.size())
        {
            processes[i + 1].start_time = total_time;

            // If the next process has not arrived yet, there will be idle time
            if (processes[i + 1].arrival_time > total_time)
            {
                std::cout << "Idle" << std::endl;

                processes[i + 1].start_time =
                    processes[i + 1].arrival_time;
            }
        }
    }

    // Calculating and printing the average wait time
    avg_time = static_cast<double>(total_wait) / num_processes;

    std::cout << "Average wait time: " << avg_time << std::endl;
}


// Shortest Job First (SJF) Algorithm
// -----------------------------------------
// The shortest burst time is evaluated first

void SJF(std::string input_file)
{
    // Variables necessary for the algorithm
    // *** THESE WILL BE REUSED FOR THE REMAINDER OF
    // THE PROGRAM ***

    int num_complete = 0;
    int total_time = 0;
    int total_wait = 0;
    double avg_time = 0;
    std::vector<Process> processes;

    // Open a file with read permission
    std::ifstream file(input_file);

    // Getting the number of processes
    int num_processes;
    file >> num_processes;

    // Creating processes with the information given
    // (This information is the same amongst
    // all three algorithms)

    for (int i = 0; i < num_processes; i++)
    {
        int id, atime, btime;
        file >> id >> atime >> btime;

        Process proc(id, atime, btime);

        // Adding the processes to an array
        processes.push_back(proc);
    }

    // Ensuring we stay within the array
    while (num_complete < num_processes)
    {
        // Initializing variables to help find the smallest process
        int smallest = 0;
        int smallest_process = -1;

        // Iterating through the array to find the process
        // with the smallest burst time that has arrived
        for (size_t i = 0; i < processes.size(); i++)
        {
            Process& proc = processes[i];

            // Making sure the process is not complete
            if (!proc.is_complete &&
                proc.arrival_time <= total_time)
            {
                // Checking if this process has a smaller burst time
                // than the current smallest
                if (proc.burst_time < smallest || smallest == 0)
                {
                    // If so, update smallest
                    smallest = proc.burst_time;
                    smallest_process = i;
                }
            }
        }

        // Selecting the smallest process to run next
        Process& proc = processes[smallest_process];

        // Outputting the start of the process
        std::cout << "Starting Process " << proc.pid
                  << ": Starting time: " << total_time << "\n"
                  << std::endl;

        // Making the process' start time equal to the total time so far
        proc.start_time = total_time;

        total_time += proc.burst_time;
        proc.end_time = total_time;

        // Calculating the wait time for the process
        proc.wait_time = proc.start_time - proc.arrival_time;
        total_wait += proc.wait_time;

        // Finishing the process
        proc.is_complete = true;
        num_complete++;

        std::cout << "Process " << proc.pid << " has finished\n"
                  << std::endl;
    }

    // Calculating and outputting the average wait time
    avg_time = static_cast<double>(total_wait) / num_processes;

    std::cout << "Average wait time: " << avg_time << std::endl;
}


// Round Robin (RR) Algorithm
// -----------------------------------------
// Each process is given a fixed time quantum
// to process

void RR(std::string input_file, int quantum)
{
    // Variables necessary for the algorithm
    // *** THESE WILL BE REUSED FOR THE REMAINDER OF
    // THE PROGRAM ***

    int num_complete = 0;
    int total_time = 0;
    int total_wait = 0;
    double avg_time = 0;
    std::vector<Process> processes;

    // Opening a file with read permissions
    std::ifstream file(input_file);

    // Getting the number of processes
    int num_processes;
    file >> num_processes;

    // Iterating through the processes and creating Process objects
    for (int i = 0; i < num_processes; i++)
    {
        int id, atime, btime;
        file >> id >> atime >> btime;

        Process proc(id, atime, btime, btime);

        // Adding these new processes to an array
        processes.push_back(proc);
    }

    // Starting the Round Robin scheduling loop
    while (num_complete < num_processes)
    {
        for (size_t i = 0; i < processes.size(); i++)
        {
            Process& proc = processes[i];

            // Making sure we are only grabbing incomplete processes,
            // processes that have actually arrived, and
            // that we still have processes to grab
            if (proc.is_complete == false &&
                proc.remaining > 0 &&
                proc.arrival_time <= total_time)
            {
                // Outputting the start of the process
                std::cout << "Starting Process " << proc.pid
                          << ": Starting time: " << total_time << "\n"
                          << std::endl;

                // If the process is larger than or the same as the quantum...
                if (proc.remaining >= quantum)
                {
                    // Simply subtract the quantum from the remaining time
                    // and adjust the total time accordingly
                    proc.remaining -= quantum;
                    total_time += quantum;
                }

                // If the process is less than the quantum...
                else if (proc.remaining < quantum &&
                         proc.remaining > 0)
                {
                    // Add the remaining time to the total time and THEN
                    // remove the remaining time
                    total_time += proc.remaining;
                    proc.remaining -= proc.remaining;
                }

                // If the process has no more time remaining...
                if (proc.remaining == 0)
                {
                    // It is complete
                    proc.is_complete = true;

                    // Calculate the wait time
                    proc.end_time = total_time;
                    proc.wait_time =
                        (proc.end_time -
                         proc.arrival_time -
                         proc.burst_time);

                    total_wait += proc.wait_time;

                    // Output that it is finished and update
                    // the number of completed processes
                    std::cout << "Process " << proc.pid
                              << " has finished\n"
                              << std::endl;

                    num_complete++;
                }
            }
        }
    }

    // Calculate the average wait time
    avg_time = static_cast<double>(total_wait) / num_processes;

    std::cout << "Average wait time: " << avg_time << std::endl;
}


int main(int argc, char* argv[])
{
    // If the user did not put in the required amount of objects, tell them
    if (argc < 3)
    {
        std::cout
            << "Usage: proj2.exe input_file [FCFS|RR|SJF] [time_quantum]"
            << std::endl;

        return 0;
    }

    // Using argv(), which I did not know was in Python,
    // to extract important pieces
    std::string input_file = argv[1];
    std::string algorithm = argv[2];

    // Determine which algorithm to use
    if (algorithm == "FCFS")
    {
        FCFS(input_file);
    }

    else if (algorithm == "SJF")
    {
        SJF(input_file);
    }

    else if (algorithm == "RR")
    {
        // Check to see if user included a time quantum
        if (argc < 4)
        {
            std::cout
                << "Round Robin requires a time quantum."
                << std::endl;

            return 0;
        }

        int quantum = std::stoi(argv[3]);

        RR(input_file, quantum);
    }

    // If it is anything else, the user is wrong
    else
    {
        std::cout << "Invalid scheduling algorithm." << std::endl;
        std::cout << "Use FCFS, SJF, or RR." << std::endl;
    }

    return 0;
}
