#pragma once

enum LogSeverity
{
    debug,
    trace,
    info,
    warn,
    error,
    fatal
};

void Log(int severity, const char* logMessage, ...);

void Debug   (const char* logMessage, ...);
void Trace   (const char* logMessage, ...);
void Info    (const char* logMessage, ...);
void Warn    (const char* logMessage, ...);
void Error   (const char* logMessage, ...);
void Fatal   (const char* logMessage, ...);