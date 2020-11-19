#include "helpers.h"
#include "math.h"

int const GRID_COUNT = 9;
int const DIMENSION = 2;

int const COLOR_CHANNEL_MIN = 0;
int const COLOR_CHANNEL_MAX = 255;

int const TRUE = 1;
int const FALSE = 0;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *rgb_values = &image[i][j];
            BYTE grey_value = round((rgb_values->rgbtRed + rgb_values->rgbtBlue + rgb_values->rgbtGreen) / 3.0);
            rgb_values->rgbtRed = grey_value;
            rgb_values->rgbtBlue = grey_value;
            rgb_values->rgbtGreen = grey_value;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Is pixel in bounds?
int is_in_bounds(int height, int width, int x, int y)
{
    if (y >= 0 && y < height && x >= 0 && x < width)
    {
        return TRUE;
    }

    return FALSE;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int const GRID[9][2] =
    {
        { -1, -1 }, // Top left corner
        { 0, -1 },  // Top
        { 1, -1 },  // Top right corner
        { -1, 0 },  // Left
        { 0, 0 },   // Middle
        { 1, 0 },   // Right
        { -1, 1 },  // Bottom left corner
        { 0, 1 },   // Bottom
        { 1, 1 }    // Bottom right corner
    };

    RGBTRIPLE temp_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long result_red = 0;
            long result_green = 0;
            long result_blue = 0;
            int pixel_count = 0; // Total number of pixels found

            for (int k = 0; k < GRID_COUNT; k++)
            {
                int x = j + GRID[k][0];
                int y = i + GRID[k][1];

                if (is_in_bounds(height, width, x, y) == TRUE)
                {
                    result_red += image[y][x].rgbtRed;
                    result_green += image[y][x].rgbtGreen;
                    result_blue += image[y][x].rgbtBlue;

                    pixel_count++;
                }
            }

            temp_image[i][j].rgbtRed = round(result_red / (float) pixel_count);
            temp_image[i][j].rgbtGreen = round(result_green / (float) pixel_count);
            temp_image[i][j].rgbtBlue = round(result_blue / (float) pixel_count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
        }
    }
}

int max(int x, int y)
{
    if (x > y)
    {
        return x;
    }

    return y;
}

int min(int x, int y)
{
    if (x < y)
    {
        return x;
    }

    return y;
}

int minmax_of_color(int num)
{
    return max(min(num, COLOR_CHANNEL_MAX), COLOR_CHANNEL_MIN);
}

int sobel_operator(long numx, long numy)
{
    return minmax_of_color(round(sqrt(pow(numx, 2.0) + pow(numy, 2.0))));
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int const GRID[9][2] =
    {
        { -1, -1 }, // Top left corner
        { 0, -1 },  // Top
        { 1, -1 },  // Top right corner
        { -1, 0 },  // Left
        { 0, 0 },   // Middle
        { 1, 0 },   // Right
        { -1, 1 },  // Bottom left corner
        { 0, 1 },   // Bottom
        { 1, 1 }    // Bottom right corner
    };

    int const GX[9] =
    {
        -1,
            0,
            1,
            -2,
            0,
            2,
            -1,
            0,
            1
        };
    int const GY[9] =
    {
        -1,
            -2,
            -1,
            0,
            0,
            0,
            1,
            2,
            1
        };

    RGBTRIPLE temp_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long resultx_red = 0;
            long resultx_green = 0;
            long resultx_blue = 0;

            long resulty_red = 0;
            long resulty_green = 0;
            long resulty_blue = 0;

            for (int k = 0; k < GRID_COUNT; k++)
            {
                int x = j + GRID[k][0];
                int y = i + GRID[k][1];

                if (is_in_bounds(height, width, x, y))
                {
                    resultx_red += image[y][x].rgbtRed * GX[k];
                    resultx_green += image[y][x].rgbtGreen * GX[k];
                    resultx_blue += image[y][x].rgbtBlue * GX[k];

                    resulty_red += image[y][x].rgbtRed * GY[k];
                    resulty_green += image[y][x].rgbtGreen * GY[k];
                    resulty_blue += image[y][x].rgbtBlue * GY[k];
                }
            }

            temp_image[i][j].rgbtRed = sobel_operator(resultx_red, resulty_red);
            temp_image[i][j].rgbtGreen = sobel_operator(resultx_green, resulty_green);
            temp_image[i][j].rgbtBlue = sobel_operator(resultx_blue, resulty_blue);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
        }
    }
}
