#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/tap-functions
plan_no_plan

is "$(./subcommands foo)" 'executing subcommand foo
argc: 1
argv[0