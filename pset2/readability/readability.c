#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

// Counts the number of letters in the text
int count_letters(string text);

// Counts the number of words in the text
int count_words(string text);

// Counts the number of sentences in the text
int count_sentences(string text);

// The average number of letters or sentences per 100 words. The count variable is either for the number of letters or number of sentences.
float get_average(int count, int word_count);

// The average number of sentences per 100 words
float get_sentence_average(int sentence_count, int word_count);

// The Coleman-Liau index is the formula used to determine the reading level of a given text
int get_coleman_liau_index(string text);

// Returns true if character is an alphabetic character
bool is_letter(char ch);

// Returns true if character is a space character
bool is_space(char ch);

bool is_punctuation(char ch);

int main(void)
{
    string text = get_string("Enter some text: ");
    int coleman_liau_index = get_coleman_liau_index(text);

    // Text before grade 1
    if (coleman_liau_index < 1)
    {
        printf("Before Grade 1\n");
    }
    // Text during and after senior undergraduate
    else if (coleman_liau_index >= 16)
    {
        printf("Grade 16+\n");
    }
    // Everything else
    else
    {
        printf("Grade %d\n", coleman_liau_index);
    }
}

int count_letters(string text)
{
    int sum = 0;
    for (int i = 0, text_length = strlen(text); i < text_length; i++)
    {
        if (is_letter(text[i]))
        {
            sum++;
        }
    }

    return sum;
}

int count_words(string text)
{
    int sum = 1;
    for (int i = 0, text_length = strlen(text); i < text_length; i++)
    {
        if (is_space(text[i]))
        {
            sum++;
        }
    }

    return sum;
}

int count_sentences(string text)
{
    int sum = 0;
    for (int i = 0, text_length = strlen(text); i < text_length; i++)
    {
        if (is_punctuation(text[i]))
        {
            sum++;
        }
    }

    return sum;
}

float get_average(int count, int word_count)
{
    return 100.0 / word_count * count;
}

int get_coleman_liau_index(string text)
{
    const float L_COEFFICIENT = 0.0588;
    const float S_COEFFICIENT = 0.296;
    const float OFFSET = 15.8;

    int letter_count = count_letters(text);
    int word_count = count_words(text);
    int sentence_count = count_sentences(text);

    float letter_average = get_average(letter_count, word_count);
    float sentence_average = get_average(sentence_count, word_count);

    return round(L_COEFFICIENT * letter_average - S_COEFFICIENT * sentence_average - OFFSET);
}

bool is_letter(char ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

bool is_space(char ch)
{
    return ch == ' ';
}

bool is_punctuation(char ch)
{
    return ch == '.' || ch == '!' || ch == '?';
}
