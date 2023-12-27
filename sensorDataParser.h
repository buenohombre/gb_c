#ifndef C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_SENSORDATAPARSER_H_
#define C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_SENSORDATAPARSER_H_

#include "sensorData.h"
#include "stdio.h"

typedef struct
{
    const char* begin;
    const char* end;
    int32_t current;
    int error;
} IntIterator;

typedef struct
{
    char buf[256];
    FILE* file;
    char* current;
    int error;
} FileIterator;

uint8_t NextColumn(IntIterator *iter);
IntIterator BeginLine(const char* str);
uint8_t NextLine(FileIterator *iter);
FileIterator BeginFile(const char* name);
void CloseFile(FileIterator* iter);
SensorData ParseLine(const char *str);


#endif // C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_SENSORDATAPARSER_H_