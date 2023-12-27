#include "args.h"
#include "stdio.h"
#include "string.h"

int ParseArgs(int argc, char **argv, ProgrammArg *dest, int maxCount)
{
    if (argc < 2)
    {
        return 0;
    }

    int arg = 0;
    int key = 0;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (key)
            {
                arg++;
                if (arg >= maxCount)
                {
                    break;
                }
            }
            key = 1;
            snprintf(dest[arg].name, sizeof(dest[arg].name), "%s", argv[i]);
        }
        else
        {
            snprintf(dest[arg].value, sizeof(dest[arg].value), "%s", argv[i]);

            if (key)
            {
                key = 0;
                arg++;
            }
        }
    }

    if(key)
    {
        arg++;
    }

    return arg;
}

int ContainKey(const ProgrammArg *args, int count, const char *key)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(args[i].name, key) == 0)
        {
            return 1;
        }
    }

    return 0;
}

const ProgrammArg *FindKey(const ProgrammArg *args, int count, const char *key)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(args[i].name, key) == 0)
        {
            return args + i;
        }
    }

    return NULL;
}