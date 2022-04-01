#include <lev/core/util.h>
#include <lev/core/app.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILE_EXT ".levlog"

using namespace lev;

namespace
{
    const char* g_directory;
    char g_curr_file[256];
}

bool Log::init()
{
    if (g_directory)
    {
        u64 time_seconds = static_cast<long>(time(NULL));
        char time_seconds_str[16];

        StrUtil::fromint(time_seconds_str, time_seconds, 10);
        StrUtil::copy(g_curr_file, g_directory, 231);
        StrUtil::copy(g_curr_file, time_seconds_str, 16);
        StrUtil::copy(g_curr_file, FILE_EXT, 8);
        g_curr_file[255] = '\0';
    }

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
    vsprintf(format, fmt, valist);
    printf("%s\n", format);
    va_end(valist);

    if (App::config().on_log)
        App::config().on_log(format, LOG_TYPE_NORMAL);
}

void Log::warn(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    vsprintf(format, fmt, valist);
    printf("\033[0;33m[WARN] %s\033[0m\n", format);
    va_end(valist);

    if (App::config().on_log)
        App::config().on_log(format, LOG_TYPE_WARN);
}

void Log::error(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    vsprintf(format, fmt, valist);
    printf("\033[0;31m[ERROR] %s\033[0m\n", format);
    va_end(valist);

    if (App::config().on_log)
        App::config().on_log(format, LOG_TYPE_ERROR);
}

void Log::file(const char *msg, ...)
{
    if (!g_directory)
        return;

    FILE *f = fopen(g_curr_file, "a");
    
    va_list args;
    va_start(args, msg);
    
    char format[100];
    vsprintf(format, msg, args);
    
    if (f)
    {
        fprintf(f, "%s\n", format);
        fclose(f);

        if (App::config().on_log)
            App::config().on_log(format, LOG_TYPE_FILE);
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