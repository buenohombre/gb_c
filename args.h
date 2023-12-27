#ifndef C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_ARGS_H_
#define C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_ARGS_H_

typedef struct
{
    char name[128];
    char value[128];
} ProgrammArg;

int ParseArgs(int argc, char **argv, ProgrammArg *dest, int maxCount);
int ContainKey(const ProgrammArg *args, int count, const char *key);
const ProgrammArg *FindKey(const ProgrammArg *args, int count, const char *key);

#endif // C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_ARGS_H_