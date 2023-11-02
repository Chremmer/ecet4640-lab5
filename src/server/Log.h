#ifndef Log_h
#define Log_h

#include <stdio.h>

/**
 * \defgroup Log
 * \brief Handles logging; can be to the console or a file.
 * @{
 * \details Log was initially going to also maintain a log.txt file that recorded user interactions with the server, but this feature was not finished. For now it just logs to the console.
 * 
 * @todo Be able to switch to file logging
 * @todo Be able to set log levels
*/

/**
 * @brief These define the levels that a log can be printed as
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
    /// @brief minimum level that the log should be in order to print stdout
    char printLevel;
    /// @brief minimum level that the log should be for it to be printed to log file
    char logLevel;
    /// @brief if not 0 will print to stdout as well as ostream
    char printAllToStdOut;

} LogSettings;

//set up functions that print to stdout no matter if printtostdout is set to true

/**
 * @brief Logs at a FATAL level the formatted string and parameters of the string
 * 
 * @param format formatted string
 * @param ... what to put in the formatted string
 */
void LogfFatal(const char * format, ...);

/**
 *  Is used
 * @brief Logs at an ERROR level the formatted string and parameters of the string
 * 
 * @param format formatted string
 * @param ... what to put in the formatted string
 */
void LogfError(const char * format, ...);

/**
 * @brief Logs at a WARNING level the formatted string and parameters of the string
 * 
 * @param format formatted string
 * @param ... what to put in the formatted string
 */
void LogfWarning(const char * format, ...);

/**
 * @brief Logs at a INFO level the formatted string and parameters of the string
 * 
 * @param format formatted string
 * @param ... what to put in the formatted string
 */
void LogfInfo(const char * format, ...);

/**
 * @brief Logs at a DEBUG level the formatted string and parameters of the string
 * 
 * @param format formatted string
 * @param ... what to put in the formatted string
 */
void LogfDebug(const char* format, ...);

/**
 * @brief Logs at a TRACE level the formatted string and parameters of the string
 * 
 * @param format formatted string
 * @param ... what to put in the formatted string
 */
void LogfTrace(const char * format, ...);

/**
 * @brief Instantiates the logger settings to run off of, must be called before logging can occur
 * 
 * @param _printStream output stream to print at
 * @param outputLevel mimumum level needed to print to the printstream
 * @param logLevel minimum level needed to print to log file
 * @param printAllToStdOut will print everyting to stdout if not set to 0 as well as _printStream
 */
void InitializeLogger(FILE* _printStream, char printLevel, char logLevel, char printAllToStdOut);


/**
 * @}
*/
#endif 
