#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

// Prints the pyramid
void print_pyramid(int height);

// Prints a line in the pyramid
void print_line(int base_size, int block_count);

//Returns the left padding of a line
string get_padding(int base_size, int block_count);

// Returns the blocks on a line
string get_blocks(int block_count);

int main(void)
{
    int height;
    // Height must be between 1 (inclusive) and 9 (exclusive)
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height >= 9);

    print_pyramid(height);
}

void print_pyramid(int height)
{
    for (int i = 1; i <= height; i++)
    {
        print_line(height, i);
    }
}

void print_line(int base_size, int block_count)
{
    string padding = get_padding(base_size, block_count);
    string blocks = get_blocks(block_count);
    printf("%s%s  %s\n", padding, blocks, blocks);
}

string get_padding(int base_size, int block_count)
{
    string padding = malloc(base_size - block_count);
    for (int i = 0; i < base_size - block_count; i++)
    {
        padding[i] = ' ';
    }
    return padding;
}

string get_blocks(int block_count)
{
    string blocks = malloc(block_count);
    for (int i = 0; i < block_count; i++)
    {
        blocks[i] = '#';
    }
    return blocks;
}
