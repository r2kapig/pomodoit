
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argparse.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static const char *const usages[] = {
    "subcommands [options] [cmd] [args]",
    NULL,