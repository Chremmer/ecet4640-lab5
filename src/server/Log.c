/**
 * \addtogroup Log
*/
#include "Log.h"

LogSettings L;

void InitializeLogger(FILE* _printStream, char outputLevel, char printToStdOut) {
    L.ostream = _printStream;
    L.outputLevel = outputLevel;
    L.printToStdOut = printToStdOut;
}

void _logf(int level, const char * format, va_list argptr) {

    // TODO add logic to include timestamp to the output
    if(level >= L.outputLevel) {
        vfprintf(L.ostream, format, argptr);
    }

    if(L.printToStdOut != 0) { //should only print if the output meets the required level as well?
        vfprintf(stdout, format, argptr);
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