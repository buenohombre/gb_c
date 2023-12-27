#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stat.h"
#include "args.h"

static void PrintUsage()
{
    printf("Usage: \r\n");
    printf("-f: input file\r\n");
    printf("-m <month number>: number of month for stat\r\n");
}

void main(int argc, char **argv)
{
    ProgrammArg args[10];
    int keyCount = ParseArgs(argc, argv, args, sizeof(args) / sizeof(*args));
    if (keyCount < 1 || ContainKey(args, keyCount, "-h"))
    {
        printf("Print temperature statistic for file\r\n");
        PrintUsage();
    }
    else
    {
        const ProgrammArg *fileArg = FindKey(args, keyCount, "-f");
        if (fileArg == NULL || strlen(fileArg->value) == 0)
        {
            printf("-f option is not specified\r\n");
            PrintUsage();
        }
        else
        {
            const ProgrammArg *monthArg = FindKey(args, keyCount, "-m");
            if (monthArg != NULL)
            {
                uint8_t month = (uint8_t)strtol(monthArg->value, NULL, 10);
                if (month <= 0 || month > 12)
                {
                    printf("Not corrent month %s\r\n", monthArg->value);
                }
                else
                {
                    printf("========== Temperature for month %02d ==============\r\n", month);
                    TempOneMonth(fileArg->value, month);
                }
            }
            else
            {
                printf("========== Temperature per month ==============\r\n");
                TempPerMonth(fileArg->value);
                printf("========== Temperature per year  ==============\r\n");
                TempPerYear(fileArg->value);
            }
        }
    }
}