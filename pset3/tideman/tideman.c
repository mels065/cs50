#include <cs50.h>
#include <string.h>
#include <stdio.h>

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
void mergesort(int min, int max);
void merge(int min, int mid, int max);
void lock_pairs(void);
bool does_cycle_exist(int candidate, int next_candidate, bool visited[]);
void print_winner(void);
int anyone_above_them(int candidate);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // Record the rank order and return true
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Use candidate_count as the range limit since we are only dealing with `candidate_count` number of people
    for (int i = 0; i < candidate_count; i++)
    {
        // Only count candidates afters the current one in `i` since we would have already visited any of them before it
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Only count candidates afters the current one in `i` since we would have already visited any of them before it
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Check if no tie
            if (preferences[i][j] != preferences[j][i])
            {
                // Does candidate i have the stronger claim?
                if (preferences[i][j] >= preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                }
                else
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                }
                // Increment pair count
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Implement mergesort
    mergesort(0, pair_count);
}

// Utilizes mergesort recursively
void mergesort(int min, int max)
{
    // Base case for when there is only one element or less
    if ((max - min) <= 1)
    {
        return;
    }
    // Get index for the middle of current range being viewed
    int mid = (min + max) / 2;
    // Sort left side of array
    mergesort(min, mid);
    // Sort right side of array
    mergesort(mid, max);
    // Merge together sub-arrays
    merge(min, mid, max);
}

// Merging two sub-arrays of an array
void merge(int min, int mid, int max)
{
    // The length of the subarray
    int n = max - min;
    // Temporary array for the new order of pairs
    pair new_pairs[n];
    // i is index of left subarray
    int i = min;
    // j is index of right subarray
    int j = mid;
    // k is index of new_pairs
    int k = 0;

    // Repeat while both indexes of the subarrays are less than their lengths
    while (i < mid && j < max)
    {
        // Get candidates of the current index in left subarray
        int winner1 = pairs[i].winner;
        int loser1 = pairs[i].loser;

        // Get candidates of the current index in right subarray
        int winner2 = pairs[j].winner;
        int loser2 = pairs[j].loser;

        // When the pair in the left subarray is stronger, add it to the new_pairs array and increment i
        if (preferences[winner1][loser1] >= preferences[winner2][loser2])
        {
            new_pairs[k] = pairs[i];
            i++;
        }
        else
        {
            new_pairs[k] = pairs[j];
            j++;
        }

        // Increment k since we have added a pair
        k++;
    }

    // Add all the elements of left subarray if any remain
    while (i < mid)
    {
        new_pairs[k] = pairs[i];
        i++;
        k++;
    }
    // Add all the elements of right subarray if any remain
    while (j < max)
    {
        new_pairs[k] = pairs[j];
        j++;
        k++;
    }

    // Record all new sorted arrangements in the pairs array
    for (int l = 0; l < n; l++)
    {
        pairs[l + min] = new_pairs[l];
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        bool visited[candidate_count];
        for (int j = 0; j < candidate_count; j++)
        {
            visited[j] = false;
        }
        // If cycle does not exist, then lock the pair.
        if (!does_cycle_exist(winner, loser, visited))
        {
            locked[winner][loser] = true;
        }
    }
}

bool does_cycle_exist(int candidate, int next_candidate, bool visited[])
{
    visited[next_candidate] = true;
    if (candidate == next_candidate)
    {
        return true;
    }

    // Check who they won against
    for (int i = 0; i < candidate_count; i++)
    {
        if (!visited[i] && locked[next_candidate][i] && does_cycle_exist(candidate, i, visited))
        {
            return true;
        }
    }

    // Check who they lost against
    for (int i = 0; i < candidate_count; i++)
    {
        if (!visited[i] && locked[i][next_candidate] && does_cycle_exist(candidate, i, visited))
        {
            return true;
        }
    }

    return false;

}

// Print the winner of the election
void print_winner(void)
{
    // The candidate who is currently in the lead
    int top_candidate;
    // This is whoever is above the current top_candidate
    int above_them = pairs[0].winner;

    // Check if anyone is above the current top_candidate. If there isn't, we have found the true top candidate.
    do
    {
        top_candidate = above_them;
        above_them = anyone_above_them(top_candidate);
    }
    while (above_them != -1);

    // Print the top candidate
    printf("%s\n", candidates[top_candidate]);
}

// Returns the index of a candidate who has a stronger pair matching above the given candidate. Returns -1 if there is no such candidate to return
int anyone_above_them(int candidate)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Ignore own index
        if (candidate == i)
        {
            continue;
        }
        // Return i if there is a locked pair in which i beats the candidate
        if (locked[i][candidate])
        {
            return i;
        }
    }

    // No one was found; the candidate is the top candidate.
    return -1;
}
