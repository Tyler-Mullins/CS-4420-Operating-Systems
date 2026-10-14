
// Include statements and standard library
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// Main function
int main()
{
    // Some helpful variables
    int num_voters = 0;
    int num_rounds = 0;
    int voter_id = -1;

    // Inputting number of voters
    cout << "Enter the number of voters: ";
    cin >> num_voters;

    // Restriction is there must be at least 3 voters
    if (num_voters < 3)
    {
        cout << "Error: The number of voters must be at least 3." << endl;
        cout << "Enter the number of voters: ";
        cin >> num_voters;
    }

    // Inputting the number of voting rounds
    cout << "Enter the number of rounds: ";
    cin >> num_rounds;

    // Initializing the pipes. One four each voter and one for
    // the signal to each voter so they can go in chronological
    // order.
    int (*pipes)[2] = new int[num_voters][2];
    int (*signals)[2] = new int[num_voters][2];

    //  Creating the pipes for each voter and signal
    for (int i = 0; i < num_voters; i++)
    {
        pipe(pipes[i]);
        pipe(signals[i]);
    }

    // Forking the voters
    for (int j = 0; j < num_voters; j++)
    {
        pid_t pid = fork();

        // Making sure it is a child
        if (pid == 0)
        {
            // Setting it to the voter id
            // and seeding a random number
            // for the voting process
            voter_id = j;
            srand(time(nullptr) ^ getpid());
            break;
        }
    }

    // This is the judge process
    if (voter_id == -1)
    {
        // Looping through the number of rounds
        for (int k = 0; k < num_rounds; k++)
        {
            // Initializing the counters for
            // the yes and no votes
            int yes_votes = 0;
            int no_votes = 0;

            // Outputting the round number
            cout << "\n------Round " << k + 1 << " ------"<< endl;
            
            // Looping through the number of voters
            for (int i = 0; i < num_voters; i++)
            {
                // Sending the signal to the voter to go
                int go = 1;
                write(signals[i][1], &go, sizeof(go));

                // Reading the vote from the voter
                int vote;
                read(pipes[i][0], &vote, sizeof(vote));

                // Outputting the vote of the voter
                cout << "Round " << k + 1
                    << ": Voter " << i << " votes: "
                    << (vote == 1 ? "YES" : "NO") << endl;

                // Counting the votes
                if (vote == 1)
                    yes_votes++;
                else
                    no_votes++;
            }

            // Outputting the result of the round
            if (yes_votes > no_votes)
                cout << "---Round " << k + 1
                    << ": The voting result is a YES" << endl;
            else if (no_votes > yes_votes)
                cout << "---Round " << k + 1
                    << ": The voting result is a NO" << endl;
            else
                cout << "---Round " << k + 1
                    << ": The voting result is a TIE" << endl;
        }
    }

    // This is the voter process
    else
    {
        // Looping through the number of rounds
        for (int k = 0; k < num_rounds; k++)
        {
            // Waiting for the signal from the judge to go
            int go;
            read(signals[voter_id][0], &go, sizeof(go));

            // Generating a random vote (0 or 1) and sending it to the judge
            int vote = rand() % 2;
            write(pipes[voter_id][1], &vote, sizeof(vote));
        }
    }

    return 0;
}