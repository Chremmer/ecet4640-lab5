/**
 * @addtogroup util
 * @{
*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "Util.h"

void printRed(const char * format, ...) {
    printf(COLOR_RED);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(COLOR_RESET);
}

void printGreen(const char * format, ...) {
    printf(COLOR_GREEN);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(COLOR_RESET);
}

void printYellow(const char * format, ...) {
    printf(COLOR_YELLOW);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(COLOR_RESET);
}

int RandomInteger(int min, int max)
{
    int r_add = rand() % (max - min + 1);
    return r_add + min;
}

float RandomFloat(float min, float max)
{
    float dif = max - min;
    int rand_int = rand() % (int)(dif * 10000);
    return min + (float)rand_int / 10000.0;
}

short RandomFlag(float percentage_chance)
{
    float random_value = (float)rand() / RAND_MAX;
    if (random_value < percentage_chance)
    {
        return 1;
    }
    return 0;
}

/**
 * @}
*/