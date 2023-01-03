
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argparse.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static const char *const usages[] = {
    "subcommands [options] [cmd] [args]",
    NULL,
};

struct cmd_struct {
    const char *cmd;
    int (*fn) (int, const char **);
};

int
cmd_foo(int argc, const char **argv)
{
    printf("executing subcommand foo\n");
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, *(argv + i));
    }
    int force = 0;
    int test = 0;
    const char *path = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL, 0, 0),
        OPT_BOOLEAN('t', "test", &test, "test only", NULL, 0, 0),
        OPT_STRING('p', "path", &path, "path to read", NULL, 0, 0),
        OPT_END(),
    };