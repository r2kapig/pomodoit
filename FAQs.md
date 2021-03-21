
# FAQs

## Why removing parsed command-line switches/options?

It destroys the original `argv` array, not compatible with other arguments parsing
library.