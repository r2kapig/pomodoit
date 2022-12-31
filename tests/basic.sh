
#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/tap-functions
plan_no_plan

is "$(./basic -f --path=/path/to/file a 2>&1)" 'force: 1
path: /path/to/file
argc: 1
argv[0]: a'

is "$(./basic -f -f --force --no-force 2>&1)" 'force: 2'

is "$(./basic -i 2>&1)" 'error: option `-i` requires a value'

is "$(./basic -i 2 2>&1)" 'int_num: 2'

is "$(./basic -i2 2>&1)" 'int_num: 2'

is "$(./basic -ia 2>&1)" 'error: option `-i` expects an integer value'

is "$(./basic -i 0xFFFFFFFFFFFFFFFFF 2>&1)" \
   'error: option `-i` numerical result out of range'

is "$(./basic -s 2.4 2>&1)" 'flt_num: 2.4'

is "$(./basic -s2.4 2>&1)" 'flt_num: 2.4'

is "$(./basic -sa 2>&1)" 'error: option `-s` expects a numerical value'

is "$(./basic -s 1e999 2>&1)" \