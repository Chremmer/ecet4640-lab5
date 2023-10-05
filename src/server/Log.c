/**
 * \addtogroup Log
*/
#include "Log.h"
#include <stdarg.h>

LogSettings L;

void InitializeLogger(FILE* _printStream, char printLevel, char logLevel, char printAllToStdOut) {
    L.ostream = _printStream;
    L.printLevel = printLevel;
    L.logLevel = logLevel;
    L.printAllToStdOut = printAllToStdOut;
}

void _logf(int level, const char * format, va_list argptr) {

    // TODO add logic to include timestamp to the 
    // Stamp level to log
    // if printing to standard out is used print with color based on level
    if(level >= L.printLevel || L.printAllToStdOut != 0) {
        vprintf(format, argptr);
    }
    if(level >= L.logLevel) {
        vfprintf(L.ostream, format, argptr);
    }
}

void LogfFatal(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    _logf(FATAL, format, argptr); 
    va_end(argptr); 
}

void LogfError(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    _logf(ERROR, format, argptr);
    va_end(argptr); 
}

void LogfWarning(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    _logf(WARNING, format, argptr);
    va_end(argptr); 
}

void LogfInfo(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    _logf(INFO, format, argptr);
    va_end(argptr); 
}

void LogfDebug(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    _logf(DEBUG, format, argptr);
    va_end(argptr); 
}

void LogfTrace(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    _logf(TRACE, format, argptr);
    va_end(argptr); 
}