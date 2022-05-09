#include <lev/core/util.h>
#include <lev/core/app.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILE_EXT ".levlog"

using namespace lv;

bool log::init()
{
    return true;
}

void log::destroy()
{
}

void log::print(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    vsprintf(format, fmt, valist);
    printf("%s\n", format);
    va_end(valist);

    if (App::inst()->config().on_log)
        App::inst()->config().on_log(format, LOG_TYPE_NORMAL);
}

void log::warn(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    vsprintf(format, fmt, valist);
    printf("\033[0;33m[WARN] %s\033[0m\n", format);
    va_end(valist);

    if (App::inst()->config().on_log)
        App::inst()->config().on_log(format, LOG_TYPE_WARN);
}

void log::error(const char* fmt, ...)
{
    va_list valist;
    va_start(valist, fmt);
    char format[512];
    vsprintf(format, fmt, valist);
    printf("\033[0;31m[ERROR] %s\033[0m\n", format);
    va_end(valist);

    if (App::inst()->config().on_log)
        App::inst()->config().on_log(format, LOG_TYPE_ERROR);
}

/*
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

        if (App::inst()->config().on_log)
            App::inst()->config().on_log(format, LOG_TYPE_FILE);
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
*/
