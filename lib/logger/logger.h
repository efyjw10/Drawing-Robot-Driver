#pragma once

#ifndef NDEBUG
#define Debug(fmt, ...) Log(DEBUG, fmt, ##__VA_ARGS__)
#else
#define Debug(fmt, ...)
#endif

#define Trace(fmt, ...) Log(TRACE, fmt, ##__VA_ARGS__)
#define Info(fmt, ...)  Log(INFO, fmt, ##__VA_ARGS__)
#define Warn(fmt, ...)  Log(WARN, fmt, ##__VA_ARGS__)
#define Error(fmt, ...) Log(ERR, fmt, ##__VA_ARGS__)
#define Fatal(fmt, ...) Log(FATAL, fmt, ##__VA_ARGS__)

enum LogLevel
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERR,
    FATAL,
    OFF
};

void LogInit();

void FileLog(const char* log);
void Log(enum LogLevel severity, const char* fmt, ...);