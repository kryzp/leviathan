#include <lev/core/log.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILE_EXT ".levlog"

namespace
{
    const char* g_directory;
    char g_curr_file[128];

    static void full_file_path(char* dest, const char* name)
    {
        char buf[128];
        strcpy(buf, g_directory);
        strcat(buf, name);
        strcat(buf, FILE_EXT);
        strcpy(dest, buf);
    }
}

bool Log::init()
{
    time_t raw_time = time(NULL);
    long int time_seconds = (long)raw_time;
    char time_seconds_char[16];
    _itoa(time_seconds, time_seconds_char, 10);
    full_file_path(g_curr_file, time_seconds_char);
    return true;
}

void Log::destroy()
{
}

void Log::print(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    sprintf(format, "[LOG] %s\n", fmt);
    vprintf(format, valist);
    va_end(valist);
}

void Log::error(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    sprintf(format, "\033[0;31m[ERROR] %s\033[0m\n", fmt);
    vprintf(format, valist);
    va_end(valist);
}

void Log::warn(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    sprintf(format, "\033[0;33m[WARN] %s\033[0m\n", fmt);
    vprintf(format, valist);
    va_end(valist);
}

void Log::file(const char* type, const char *msg, ...)
{
    FILE *f = fopen(g_curr_file, "a");
    
    va_list args;
    va_start(args, msg);
    
    char format[100];
    vsprintf(format, msg, args);
    
    if (f)
    {
        fprintf(f, "[%s] %s\n", type, format);
        fclose(f);
    }
    
    va_end(args);
}

void Log::filenewline()
{
    FILE *f = fopen(g_curr_file, "a");
    
    if (f)
    {
        fprintf(f, "\n");
        fclose(f);
    }
}

void Log::directory(const char* dir)
{
    g_directory = dir;
}

const char* Log::directory()
{
    return g_directory;
}
