
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argparse.h"

static const char *const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

#define PERM_READ  (1<<0)
#define PERM_WRITE (1<<1)
#define PERM_EXEC  (1<<2)

int
main(int argc, const char **argv)
{
    int force = 0;
    int test = 0;
    int int_num = 0;
    float flt_num = 0.f;
    const char *path = NULL;
    int perms = 0;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL, 0, 0),
        OPT_BOOLEAN('t', "test", &test, "test only", NULL, 0, 0),
        OPT_STRING('p', "path", &path, "path to read", NULL, 0, 0),
        OPT_INTEGER('i', "int", &int_num, "selected integer", NULL, 0, 0),
        OPT_FLOAT('s', "float", &flt_num, "selected float", NULL, 0, 0),
        OPT_GROUP("Bits options"),
        OPT_BIT(0, "read", &perms, "read perm", NULL, PERM_READ, OPT_NONEG),
        OPT_BIT(0, "write", &perms, "write perm", NULL, PERM_WRITE, 0),
        OPT_BIT(0, "exec", &perms, "exec perm", NULL, PERM_EXEC, 0),
        OPT_END(),
    };
