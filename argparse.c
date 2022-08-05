/**
 * Copyright (C) 2012-2015 Yecheng Fu <cofyc.jackson at gmail dot com>
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "argparse.h"

#define OPT_UNSET 1
#define OPT_LONG  (1 << 1)

static const char *
prefix_skip(const char *str, const char *prefix)
{
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) ? NULL : str + len;
}

static int
prefix_cmp(const char *str, const char *prefix)
{
    for (;; str++, prefix++)
        if (!*prefix) {
            return 0;
        } else if (*str != *prefix) {
            return (unsigned char)*prefix - (unsigned char)*str;
        }
}

static void
argparse_error(struct argparse *self, const struct argparse_option *opt,
               const char *reason, int flags)
{
    (void)self;
    if (flags & OPT_LONG) {
        fprintf(stderr, "error: option `--%s` %s\n", opt->long_name, reason);
    } else {
        fprintf(stderr, "error: option `-%c` %s\n", opt->short_name, reason);
    }
    exit(EXIT_FAILURE);
}

static int
argparse_getvalue(struct argparse *self, const struct argparse_option *opt,
                  int flags)
{
    const char *s = NULL;
    if (!opt->value)
        goto skipped;
    switch (opt->type) {
    case ARGPARSE_OPT_BOOLEAN:
        if (flags & OPT_UNSET) {
            *(int *)opt->value = *(int *)opt->value - 1;
        } else {
            *(int *)opt->value = *(int *)opt->value + 1;
        }
        if (*(int *)opt->value < 0) {
            *(int *)opt->value = 0;
        }
        break;
    case ARGPARSE_OPT_BIT:
        if (flags & OPT_UNSET) {
            *(int *)opt->value &= ~opt->data;
        } else {
            *(int *)opt->value |= opt->data;
        }
        break;
    case ARGPARSE_OPT_STRING:
        if (self->optvalue) {
            *(const char **)opt->value = self->optvalue;
            self->optvalue             = NULL;
        } else if (self->argc > 1) {
            self->argc--;
            *(const char **)opt->value = *++self->argv;
        } else {
            argparse_error(self, opt, "requires a value", flags);
        }
        break;
    case ARGPARSE_OPT_INTEGER:
        errno = 0;
        if (self->optvalue) {
            *(int *)opt->value = strtol(self->optvalue, (char **)&s, 0);
            self->optvalue     = NULL;
        } else if (self->argc > 1) {
            self->argc--;
            *(int *)opt->value = strtol(*++self->argv, (char **)&s, 0);
        } else {
            argparse_error(self, opt, "requires a value", flags);
        }
        if (errno == ERANGE)
            argparse_error(self, opt, "numerical result out of range", flags);
        if (s[0] != '\0') // no digits or contains invalid characters
            argparse_error(self, opt, "expects an integer value", flags);
        break;
    case ARGPARSE_OPT_FLOAT:
        errno = 0;
        if (self->optvalue) {
            *(float *)opt->value = strtof(self->optvalue, (char **)&s);
            self->optvalue       = NULL;
        } else if (self->argc > 1) {
            self->argc--;
            *(float *)opt->value = strtof(*++self->argv, (char **)&s);
        } else {
            argparse_error(self, opt, "requires a value", flags);
        }
        if (errno == ERANGE)
            argparse_error(self, opt, "numerical result out of range", flags);
        if (s[0] != '\0') // no digits or contains invalid characters
            argparse_error(self, opt, "expects a numerical value", flags);
        break;
    default:
        assert(0);
    }

skipped:
    if (opt->callback) {
        return opt->callback(self, opt);
    }
    return 0;
}

static void
argparse_options_check(const struct argparse_option *options)
{
    for (; options->type != ARGPARSE_OPT_END; options++) {
        switch (options->type) {
            case ARGPARSE_OPT_END:
            case ARGPARSE_OPT_BOOLEAN:
            case ARGPARSE_OPT_BIT:
            case ARGPARSE_OPT_INTEGER:
            case ARGPARSE_OPT_FLOAT:
            case ARGPARSE_OPT_STRING:
            case ARGPARSE_OPT_GROUP:
                continue;
            default:
                fprintf(stderr, "wrong option type: %d", options->type);
                break;
        }
    }
}

static int
argparse_short_opt(struct argparse *self, const struct argparse_option *options)
{
    for (; options->type != ARGPARSE_OPT_END; options++) {
        if (options->short_name == *self->optvalue) {
            self->optvalue = self->optvalue[1] ? self->optvalue + 1 : NULL;
            return argparse_getvalue(self, options, 0);
        }
    }
    return -2;
}

static int
argparse_long_opt(struct argparse *self, const struct argparse_option *options)
{
    for (; options->type != ARGPARSE_OPT_END; options++) {
        const char *rest;
        int opt_flags = 0;
        if (!options->long_name)
            continue;

        rest = prefix_skip(self->argv[0] + 2, options->long_name);
        if (!rest) {
            // negation disabled?
            if (options->flags & OPT_NONEG) {
                continue;
            }
            // only OPT_BOOLEAN/OPT_BIT supports negation
            if (options->type != ARGPARSE_OPT_BOOLEAN && options->type !=
                ARGPARSE_OPT_BIT) {
                continue;
            }

            if (prefix_cmp(self->argv[0] + 2, "no-")) {
                continue;
            }
            rest = prefix_skip(self->argv[0] + 2 + 3, options->long_name);
            if (!rest)
                continue;
            opt_flags |= OPT_UNSET;
        }
        if (*rest) {
            if (*rest != '=')
                continue;
            self->optvalue = rest + 1;
        }
        return argparse_getvalue(self, options, opt_flags | OPT_LONG);
    }
    return -2;
}

int
argparse_init(struct argparse *self, struct argparse_option *options,
              const char *const *usages, int flags)
{
    memset(self, 0, sizeof(*self));
    self->options     = options;
    self->usages      = usages;
    self->flags       = flags;
    self->description = NULL;
    self->epilog      = NULL;
    return 0;
}

void
argparse_describe(struct argparse *self, const char *description,
                  const char *epilog)
{
    self->description = description;
    self->epilog      = epilog;
}

int
argparse_parse(struct argparse *self, int argc, const char **argv)
{
    self->argc = argc - 1;
    self->argv = argv + 1;
    self->out  = argv;

    argparse_options_check(self->options);

    for (; self->argc; self->argc--, self->argv++) {
        const char *arg = self->argv[0];
        if (arg[0] != '-' || !arg[1]) {
            if (self->flags & ARGPARSE_STOP_AT_NON_OPTION) {
                goto end;
            }
            // if it's not option or is a single char '-', copy verbatim
            self->out[self->cpidx++] = self->argv[0];
            co