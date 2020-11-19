#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

const int ALPHABET_SIZE = 26; // The total number of characters in the alphabet
const int UPPER_CASE_ORIGIN = 65; // The ASCII index for 'A'
const int LOWER_CASE_ORIGIN = 97; // The ASCII index for 'a'

// Checks if all characters in string are alphabetic characters
bool all_alpha(string str);

// Checks if no duplicates exist The case_origin is the first index for either lower or upper case
bool no_duplicates(string str, int case_origin);

// Performs a substitution on a letter. The case_origin is the first index for either lower or upper case
char substitute_letter(char ch, string key, int case_origin);

// Substitutes the string with characters from the key
string substitution(string text, string key);

int main(int argc, string argv[])
{
    // A key must be provided as the only argument
    if (argc != 2)
    {
        printf("Error: %d-letter key must be provided\n", ALPHABET_SIZE);
        return 1;
    }
    // The 26-character key must have only alphabetic characters and no duplicates
    string key = argv[1];
    if (strlen(key) != ALPHABET_SIZE || !all_alpha(key) || !no_duplicates(key, LOWER_CASE_ORIGIN)
        || !no_duplicates(key, UPPER_CASE_ORIGIN))
    {
        printf("Error: Key must be %d alphabetic characters (no digits, punctuation, etc.) and there can only be 1 of every character.\n",
               ALPHABET_SIZE);
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: %s\n", substitution(plaintext, key));
    return 0;
}

bool all_alpha(string str)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (!isalpha(str[i]))
        {
            return false;
        }
    }

    return true;
}

bool no_duplicates(string str, int case_origin)
{
    for (char alpha = (char) case_origin; alpha < (char)(case_origin + ALPHABET_SIZE); alpha++)
    {
        // Initialize a variable to count the alphabet characters, indexed from 0 ('a' and 'A') through 25 ('z' and 'Z')
        int count[ALPHABET_SIZE];
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            count[i] = 0;
        }
        for (int i = 0, n = strlen(str); i < n; i++)
        {
            if (str[i] == alpha)
            {
                int letter_index = (int) str[i] - case_origin;
                count[letter_index]++;

                if (count[letter_index] > 1)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

char substitute_letter(char ch, string key, int case_origin)
{
    // Use the case_origin as the offset to get the correct index
    return key[(int) ch - case_origin];
}

string substitution(string text, string key)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isupper(text[i]))
        {
            text[i] = toupper(substitute_letter(text[i], key, UPPER_CASE_ORIGIN));
        }
        else if (islower(text[i]))
        {
            text[i] = tolower(substitute_letter(text[i], key, LOWER_CASE_ORIGIN));
        }
    }

    return text;
}
