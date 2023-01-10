#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/tap-functions
plan_no_plan

is "$(./subcommands foo)" 'execu