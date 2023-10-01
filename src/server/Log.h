#ifndef Log_h
#define Log_h
/**
 * \defgroup Log
 * \brief Handles logging; can be to the console or a file.
 * @{
 * 
 * @todo Be able to switch to file logging
 * @todo Be able to set log levels
*/

void logfError(const char * format, ...);

void logfSuccess(const char * format, ...);

void logfWarning(const char * format, ...);

void log(const char * format, ...);

/**
 * @}
*/
#endif 