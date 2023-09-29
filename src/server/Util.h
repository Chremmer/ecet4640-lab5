#ifndef Util_h
#define Util_h
/**
 * \defgroup util
 * \brief Utility functions used by various modules but not dependent on any other modules.
 * @{
*/

/** A virtual terminal escape sequence to print foreground red. */
#define COLOR_RED "\e[38;2;255;75;75m"
/** A VTE for green. */
#define COLOR_GREEN "\e[38;2;0;240;0m"
/** A VTE to reset the printing color. */
#define COLOR_RESET "\e[0m"

/**
 * Prints to the console in red
*/
void printRed(const char * format, ...);

/**
 * Prints to the console in green
*/
void printGreen(const char * format, ...);


/**
    Returns an integer between min and max.
    @param min The minimum, inclusive.
    @param max The maximum, inclusive.
    @returns A random integer between min and max.
*/
int RandomInteger(int min, int max);

/**
    Returns a float between min and max.
    @param min The minimum, inclusive.
    @param max The maximum, inclusive.
    @returns A random integer between min and max.
*/
float RandomFloat(float min, float max);

/**
    Returns 1, percentage_chance of the time.
    @param percentage_chance The chance to return 1.
    @note If percentage_chance > 1, this will always return true.
    @returns 1 or 0
*/
short RandomFlag(float percentage_chance);

/**
 * @}
*/
#endif 