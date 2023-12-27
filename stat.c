#include "sensorDataParser.h"
#include "aggregator.h"
#include "avg.h"

extern SensorData SensorDataError;

static uint8_t MonthFilter(const SensorData *data, FilterContext *context)
{
    if (data->Error != SensorDataError.Error && data->Data.month == context->uint8_tArg1)
    {
        return 1;
    }

    return 0;
}

static uint8_t YearFilter(const SensorData *data, FilterContext *context)
{
    if (data->Error != SensorDataError.Error && data->Data.year == context->uint32_tArg1)
    {
        return 1;
    }

    return 0;
}

static SelectResult YearSelector(const SensorData *data)
{
    SelectResult result = {.uint32_tRes = 0};

    if (data->Error != SensorDataError.Error)
    {
        result.uint32_tRes = data->Data.year;
    }

    return result;
}

static uint8_t YearComparer(const SelectResult *r1, const SelectResult *r2)
{
    return r1->uint32_tRes == r2->uint32_tRes;
}

void TempPerMonth(const char *fileName)
{
    for (int i = 1; i <= 12; i++)
    {
        FileIterator fileIter = BeginFile(fileName);
        if (fileIter.file == NULL)
        {
            break;
        }

        Aggregator ag = {
            .filter = MonthFilter,
            .filterContext = {.uint8_tArg1 = i}};

        snprintf(ag.name, sizeof(ag.name), "month %02d", i);

        Aggregator *aggs[] = {&ag};
        Aggregate(&fileIter, aggs, sizeof(aggs) / sizeof(*aggs));

        if (ag.count > 0)
        {
            PrintAvg(&ag);
            PrintMin(&ag);
            PrintMax(&ag);
            printf("\r\n");
        }

        CloseFile(&fileIter);
    }
}

void TempOneMonth(const char *fileName, int month)
{
    FileIterator fileIter = BeginFile(fileName);
    if (fileIter.file == NULL)
    {
        return;
    }

    Aggregator ag = {
        .filter = MonthFilter,
        .filterContext = {.uint8_tArg1 = month}};

    snprintf(ag.name, sizeof(ag.name), "month %02d", month);

    Aggregator *aggs[] = {&ag};
    Aggregate(&fileIter, aggs, sizeof(aggs) / sizeof(*aggs));

    if (ag.count > 0)
    {
        PrintAvg(&ag);
        PrintMin(&ag);
        PrintMax(&ag);
        printf("\r\n");
    }
    else
    {
        printf("no entries found for %02d month\n\r", month);
    }

    CloseFile(&fileIter);
}

void TempPerYear(const char *fileName)
{
    FileIterator fileIter = BeginFile(fileName);
    if (fileIter.file != NULL)
    {
        Selector sel = {
            .filter = NULL,
            .resultFunc = YearSelector,
            .resultComparer = YearComparer};

        SelectDistinct(&fileIter, &sel);
        CloseFile(&fileIter);

        for (size_t i = 0; i < sel.count; i++)
        {
            uint32_t year = sel.result[i].uint32_tRes;
            FileIterator fileIter = BeginFile(fileName);
            if (fileIter.file != NULL)
            {
                Aggregator ag = {
                    .filter = YearFilter,
                    .filterContext = {.uint32_tArg1 = year}};

                snprintf(ag.name, sizeof(ag.name), "year %04d", year);

                Aggregator *aggs[] = {&ag};
                Aggregate(&fileIter, aggs, sizeof(aggs) / sizeof(*aggs));

                if (ag.count > 0)
                {
                    PrintAvg(&ag);
                    PrintMin(&ag);
                    PrintMax(&ag);
                    printf("\r\n");
                }
            }

            CloseFile(&fileIter);
        }
    }
}