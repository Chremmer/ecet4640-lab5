#ifndef Log_h
#define Log_h

#include <stdio.h>

/**
 * \defgroup Log
 * \brief Handles logging; can be to the console or a file.
 * @{
 * 
 * @todo Be able to switch to file logging
 * @todo Be able to set log levels
*/

/**
 * @brief These define the levels that a log can be printed ad
 * 
 */
#define TRACE 0
#define DEBUG 1
#define INFO 2
#define WARNING 3
#define ERROR 4
#define FATAL 5

typedef struct 
{
    /// @brief output the logger should go to
    FILE * ostream;
    /// @brief minimum level that the log should be in order to ouput it
    char outputLevel;
    /// @brief if not 0 will print to stdout as well as ostream
    char printToStdOut;

} LogSettings;


/**
 * @brief Logs at a FATAL level the formatted string and parameters of the string
 * 
 * @param format formated string
 * @param ... what to put in the formated string
 */
void LogfFatal(const char * format, ...);

/**
 * @brief Logs at an ERROR level the formatted string and parameters of the string
 * 
 * @param format formated string
 * @param ... what to put in the formated string
 */
void LogfError(const char * format, ...);

/**
 * @brief Logs at a WARNING level the formatted string and parameters of the string
 * 
 * @param format formated string
 * @param ... what to put in the formated string
 */
void LogfWarning(const char * format, ...);

/**
 * @brief Logs at a INFO level the formatted string and parameters of the string
 * 
 * @param format formated string
 * @param ... what to put in the formated string
 */
void LogfInfo(const char * format, ...);

/**
 * @brief Logs at a DEBUG level the formatted string and parameters of the string
 * 
 * @param format formated string
 * @param ... what to put in the formated string
 */
void LogfDebug(const char* format, ...);

/**
 * @brief Logs at a TRACE level the formatted string and parameters of the string
 * 
 * @param format formated string
 * @param ... what to put in the formated string
 */
void LogfTrace(const char * format, ...);

/**
 * @brief Instaiates the logger settings to run off of, must be called before logging can occur
 * 
 * @param _printStream output stream to print at
 * @param outputLevel mimumum level needed to print to the printstream
 * @param printToStdOut will print everyting to stdout if not set to 0 as well as _printStream
 */
void InitializeLogger(FILE* _printStream, char outputLevel, char printToStdOut);


/**
 * @}
*/
#endif 