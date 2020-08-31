#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // only the first n-1 ranks for the outer loop (preferred)
    for (int i = 0, n = candidate_count - 1; i < n; i++)
    {
        // only the last n-1 ranks for the inner loop (non-preferred)
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // outer loop 0 to n-1
    for (int i = 0, n = candidate_count - 1; i < n; i++)
    {
        // innerloop 0+1 to n
        for (int j = i + 1; j < candidate_count; j++)
        {
            // voters that prefer i over j
            int i_pref = preferences[i][j];
            // voters that prefer j over i
            int j_pref = preferences[j][i];
            // find higher preference and set win/lose accordingly
            // pair_count keeps track of non-draws
            if (i_pref > j_pref)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;

            }
            else if (i_pref < j_pref)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            // Last possibiity is same number is preferences
            // which we don't do anything for
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Make an array that contains the head to head win difference
    int win_strength[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        win_strength[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }
    // sorting pairs
    // sort the win strength array and at the same time the pairs array
    int winner_temp;
    int loser_temp;
    int strength_temp;
    int swaps;
    for (int i = 1; i < pair_count; i++)
    {
        swaps = 0;
        for (int j = 0, n = pair_count - i; j < n; j++)
        {
            if (win_strength[j] < win_strength[j + 1])
            {
                // swap is required
                swaps++;
                // assign lower value to temp
                strength_temp = win_strength[j];
                winner_temp = pairs[j].winner;
                loser_temp = pairs[j].loser;
                // swap higher value
                win_strength[j] = win_strength[j + 1];
                pairs[j].winner = pairs[j + 1].winner;
                pairs[j].loser = pairs[j + 1].loser;
                // swap lower value from temp
                win_strength[j + 1] = strength_temp;
                pairs[j + 1].winner = winner_temp;
                pairs[j + 1].loser = loser_temp;
            }
        }
        // exit sorting if the array is sorted before going through all iterations
        if (swaps == 0)
        {
            break;
        }
    }

    return;

}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            // do not create a cycle i.e. skip if the loser of current pair is already pointing at someone else
            bool status = locked[pairs[i].loser][j];
            if (status == true)
            {
                break;
            }
            // add the locked pair on the last iteration since there will be no cycles
            if (j == pair_count - 1)
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
            }
        }


    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // The winners are the candidates with no arrows pointing at them i.e.
    // locked[1 to n][candidate] == false
    for (int i = 0; i < candidate_count; i++)
    {
        // track if arrow pointing to candidate
        int true_count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                true_count++;
            }
        }
        // winner if no arrow pointing at candidate
        if (true_count == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

