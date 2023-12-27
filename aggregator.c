#include "sensorDataParser.h"
#include "aggregator.h"

extern SensorData SensorDataError;

void Aggregate(FileIterator *iter, Aggregator **aggs, int aggCount)
{
    while (NextLine(iter))
    {
        SensorData data = ParseLine(iter->current);
        if (data.Error == SensorDataError.Error)
        {
            printf("error sensor data in %s\n\r", iter->current);
        }

        for (int i = 0; i < aggCount; i++)
        {
            if (aggs[i]->filter == NULL || aggs[i]->filter(&data, &aggs[i]->filterContext))
            {
                if (aggs[i]->count < sizeof(aggs[i]->data) / sizeof(*(aggs[i]->data)))
                {
                    aggs[i]->data[aggs[i]->count++] = data;
                }
            }
        }
    }
}

void SelectDistinct(FileIterator *iter, Selector *selector)
{
    while (NextLine(iter))
    {
        SensorData data = ParseLine(iter->current);
        if (data.Error == SensorDataError.Error)
        {
            printf("error sensor data in %s\n\r", iter->current);
        }

        if (selector->filter == NULL || selector->filter(&data, &selector->filterContext))
        {
            SelectResult result = selector->resultFunc(&data);
            size_t i = 0;
            for (; i < selector->count; i++)
            {
                if (selector->resultComparer(&selector->result[i], &result))
                {
                    break;
                }
            }
            if (i == selector->count && selector->count < sizeof(selector->result) / sizeof(*(selector->result)))
            {
                selector->result[selector->count++] = result;
            }
        }
    }
}
