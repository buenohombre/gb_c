#ifndef C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_AGGREGATOR_H_
#define C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_AGGREGATOR_H_

#include "sensorData.h"
#include "sensorDataParser.h"

#define MaxCount 1000

typedef union
{
    uint8_t uint8_tArg1;
    uint32_t uint32_tArg1;

} FilterContext;

typedef union
{
    uint8_t uint8_tRes;
    uint32_t uint32_tRes;

} SelectResult;

typedef struct 
{
    char name[32];
    SensorData data[MaxCount];
    uint32_t count;
    uint8_t (*filter)(SensorData*, FilterContext*);
    FilterContext filterContext;
} Aggregator;

typedef struct 
{
    char name[32];
    SelectResult result[MaxCount];
    uint32_t count;
    uint8_t (*filter)(SensorData*, FilterContext*);
    FilterContext filterContext;
    SelectResult (*resultFunc)(SensorData*);
    uint8_t (*resultComparer)(SelectResult*, SelectResult*);
} Selector;

void Aggregate(FileIterator *iter, Aggregator **aggs, int aggCount);
void SelectDistinct(FileIterator *iter, Selector *selector);

#endif // C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_AGGREGATOR_H_