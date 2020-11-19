#include <stdio.h>
#include <math.h>
#include <cs50.h>

// Print card result
void print_card_result(string result);

// Get card length; returns -1 if invalid.
long get_card_length(long card_num);

// Uses the checksum formula to see if the card is valid.
bool is_checksum_valid(long card_length, long card_num);

// Step One: Double every other digit starting from second to last, and then add them together
int checksum_step_one(long card_length, long card_num);

// Step Two: Add every other digit starting from the last
int checksum_step_two(long card_length, long card_num);

// Step Three:
bool checksum_step_three(int checksum_result);

// Gets the credit card company name for a credit card; invalid if it doesn't match
string get_credit_card_name(long card_length, long card_num);

string const INVALID = "INVALID";
string const AMEX = "AMEX";
string const MASTERCARD = "MASTERCARD";
string const VISA = "VISA";

long const MIN_LENGTH = 1000000000000;
long const MAX_LENGTH = 1000000000000000;

int main(void)
{
    long card_num = get_long("Number: ");
    long card_length = get_card_length(card_num);

    if (card_length == -1 || !is_checksum_valid(card_length, card_num))
    {
        print_card_result(INVALID);
    }
    else
    {
        print_card_result(get_credit_card_name(card_length, card_num));
    }
}

void print_card_result(string result)
{
    printf("%s\n", result);
}

long get_card_length(long card_num)
{
    long card_length = MIN_LENGTH;
    do
    {
        if (card_num / card_length >= 10)
        {
            card_length *= 10;
        }
        if (card_length > MAX_LENGTH)
        {
            return -1;
        }
    }
    while (card_num / card_length >= 10);

    return card_length;
}

bool is_checksum_valid(long card_length, long card_num)
{
    // The result of adding the sums obtained in steps ones and two
    int checksum_result = checksum_step_one(card_length, card_num) + checksum_step_two(card_length, card_num);
    return checksum_step_three(checksum_result);
}

int checksum_step_one(long card_length, long card_num)
{
    long mod = 10; // Used for the modulo operator to get the digit place
    long div = 10; // Used to divide to get digit
    int sum = 0; // Total sum

    while (div <= card_length)
    {
        int result = (card_num / div % mod * 2);
        sum += (result / 10) + (result % 10);
        div *= 100;
    }

    return sum;
}

int checksum_step_two(long card_length, long card_num)
{
    long mod = 10; // Used for the modulo operator to get the digit place
    long div = 1; // Used to divide to get digit
    int sum = 0; // Total sum

    while (div <= card_length)
    {
        sum += (card_num / div % mod);
        div *= 100;
    }

    return sum;
}

bool checksum_step_three(int checksum_result)
{
    return checksum_result % 10 == 0;
}

string get_credit_card_name(long card_length, long card_num)
{
    // Lengths
    long const AMEX_LENGTH = powl(10, 14);
    long const MC_AND_VISA_LENGTH = powl(10, 15);
    long const VISA_LENGTH = powl(10, 12);

    // Starting digits of the credit cards
    int const AMEX_START_1 = 34;
    int const AMEX_START_2 = 37;
    // MasterCards can have digits that begins with a number between 51 and 55 (both inclusive)
    int const MC_START_MIN = 51;
    int const MC_START_MAX = 55;
    int VISA_START = 4;

    int first_digits = card_num / (card_length / 10);
    // Is it American Express?
    if (card_length == AMEX_LENGTH && (first_digits == AMEX_START_1 || first_digits == AMEX_START_2))
    {
        return AMEX;
    }
    // OR is it MasterCard
    else if (card_length == MC_AND_VISA_LENGTH && (first_digits >= MC_START_MIN && first_digits <= MC_START_MAX))
    {
        return MASTERCARD;
    }
    // OR is it Visa
    else if ((card_length == MC_AND_VISA_LENGTH || card_length == VISA_LENGTH) && (first_digits / 10) == VISA_START)
    {
        return VISA;
    }
    // Card length and starting digits do not exist for valid credit card; this is invalid
    else
    {
        return INVALID;
    }
}
