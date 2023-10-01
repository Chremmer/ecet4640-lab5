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

void LogfError(const char * format, ...);

void LogfSuccess(const char * format, ...);

void LogfWarning(const char * format, ...);

void Log(const char * format, ...);

/**
 * @}
*/
#endif 