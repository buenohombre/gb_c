#include "sensorData.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "stdio.h"
#include "sensorDataParser.h"

const SensorData SensorDataError = {.Error = -1};

static void SetField32(int32_t src, void *dst)
{
    *(int32_t *)dst = src;
}

static void SetField8(int32_t src, void *dst)
{
    *(int8_t *)dst = src;
}

static uint8_t ParseColumn(IntIterator *iter, const char *name, int min, int max, void *dst, void (*setFunc)(int32_t, void *))
{
    if (NextColumn(iter))
    {
        if (iter->error > 0 || iter->current < min || iter->current > max)
        {
            printf("error in %s column\n\r", name);
            return 0;
        }
        else
        {
            setFunc(iter->current, dst);
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

SensorData ParseLine(const char *str)
{
    // dddd;mm;dd;hh;mm;temperature
    char buf[256] = {0};
    const int count = snprintf(buf, sizeof(buf) - 1, "%s", str);
    if (count < 0 || count >= sizeof(buf))
    {
        return SensorDataError;
    }

    SensorData data;
    IntIterator iter = BeginLine(buf);

    if (!ParseColumn(&iter, "Year", 1900, 2100, &data.Data.year, SetField32))
    {
        return SensorDataError;
    }

    if (!ParseColumn(&iter, "Month", 1, 12, &data.Data.month, SetField8))
    {
        return SensorDataError;
    }

    if (!ParseColumn(&iter, "Day", 1, 31, &data.Data.day, SetField8))
    {
        return SensorDataError;
    }

    if (!ParseColumn(&iter, "Hour", 0, 23, &data.Data.hour, SetField8))
    {
        return SensorDataError;
    }

    if (!ParseColumn(&iter, "Minute", 0, 59, &data.Data.minute, SetField8))
    {
        return SensorDataError;
    }

    if (!ParseColumn(&iter, "Temperature", -99, 99, &data.Data.temperature, SetField8))
    {
        return SensorDataError;
    }

    return data;
}

IntIterator BeginLine(const char *str)
{
    IntIterator iter = {
        .begin = str,
        .end = NULL,
        .current = 0};

    return iter;
}

uint8_t NextColumn(IntIterator *iter)
{
    // errno can be set to any non-zero value by a library function call
    // regardless of whether there was an error, so it needs to be cleared
    // in order to check the error set by strtol
    errno = 0;
    iter->current = strtol(iter->begin, &iter->end, 10);
    if (iter->begin == iter->end)
    {
        return 0;
    }

    iter->error = errno;
    iter->begin = iter->end;

    for (; *iter->begin; iter->begin++, iter->end++)
    {
        if (*iter->begin == ';')
        {
            iter->begin++;
            iter->end++;
            break;
        }
    }

    return 1;
}

FileIterator BeginFile(const char *name)
{
    FILE *fp = fopen(name, "r");
    if (fp == NULL)
    {
        printf("file %s not found\r\n", name);
        FileIterator iter = {
            .current = NULL,
            .file = NULL,
            .error = 0};

        return iter;
    }

    FileIterator iter = {
        .current = NULL,
        .error = 0,
        .file = fp};

    return iter;
}

uint8_t NextLine(FileIterator *iter)
{
    if (feof(iter->file))
    {
        iter->current = NULL;
        iter->error = 0;
        fclose(iter->file);
        return 0;
    }

    if (!fgets(iter->buf, sizeof(iter->buf) - 1, iter->file))
    {
        iter->error = ferror(iter->file);
        iter->current = NULL;
        fclose(iter->file);
        return 0;
    }

    iter->current = iter->buf;
    iter->error = 0;

    return 1;
}

void CloseFile(FileIterator *iter)
{
    if (iter->file != NULL)
    {
        fclose(iter->file);
        iter->file = NULL;
    }
}
