#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t BYTE; // A type definition for bytes
int BUFFER_SIZE = 512; // The size of the buffer

int main(int argc, char *argv[])
{
    // Error if not exactly 1 argument
    if (argc != 2)
    {
        printf("Please provide only 1 argument: the forensic file for which to recover JPEGs\n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");

    // Error if file cannot be open
    if (file == NULL)
    {
        printf("File %s could not be read\n", argv[1]);
        return 1;
    }

    BYTE *buffer = malloc(sizeof(BYTE) * BUFFER_SIZE);
    FILE *jpg = NULL;
    int file_iteration = 0;
    // While 1 block of memory can be read
    while (fread(buffer, BUFFER_SIZE, 1, file) == 1)
    {
        // Find header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close current jpg if one is being worked on
            if (jpg != NULL)
            {
                fclose(jpg);
            }

            // Create new jpg
            char *filename = malloc(strlen("000.jpg") + 1);
            sprintf(filename, "%03i.jpg", file_iteration);
            jpg = fopen(filename, "w");
            file_iteration++;
            free(filename);
        }
        // Write to jpg if one is being worked on
        if (jpg != NULL)
        {
            fwrite(buffer, BUFFER_SIZE, 1, jpg);
        }
    }

    fclose(jpg);
    fclose(file);
    free(buffer);
}
