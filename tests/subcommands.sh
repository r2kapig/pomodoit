#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/tap-functions
plan_no_plan

is "$(./subcommands foo)" 'executing subcommand foo
argc: 1
argv[0]: foo
after argparse_parse:
argc: 0'

is "$(./subcommands foo -