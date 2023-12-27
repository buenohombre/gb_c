#include "avg.h"

int Avg(const Aggregator *ag)
{
    int result = 0;
    if (ag->count > 0)
    {
        for (size_t i = 0; i < ag->count; i++)
        {
            result += ag->data[i].Data.temperature;
        }
        result = result / (int)ag->count;
    }

    return result;
}

int Min(const Aggregator *ag)
{
    int result = 0;
    if (ag->count > 0)
    {
        result = ag->data[0].Data.temperature;
        for (size_t i = 1; i < ag->count; i++)
        {
            if (result > ag->data[i].Data.temperature)
            {
                result = ag->data[i].Data.temperature;
            }
        }
    }

    return result;
}

int Max(const Aggregator *ag)
{
    int result = 0;
    if (ag->count > 0)
    {
        result = ag->data[0].Data.temperature;
        for (size_t i = 1; i < ag->count; i++)
        {
            if (result < ag->data[i].Data.temperature)
            {
                result = ag->data[i].Data.temperature;
            }
        }
    }

    return result;
}

void PrintAvg(const Aggregator *ag)
{
    printf("avg temp for %s (%d entries): %02d\n\r", ag->name, ag->count, Avg(ag));
}

void PrintMin(const Aggregator *ag)
{
    printf("min temp for %s (%d entries): %02d\n\r", ag->name, ag->count, Min(ag));
}

void PrintMax(const Aggregator *ag)
{
    printf("max temp for %s (%d entries): %02d\n\r", ag->name, ag->count, Max(ag));
}