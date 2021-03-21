
# FAQs

## Why removing parsed command-line switches/options?

It destroys the original `argv` array, not compatible with other arguments parsing
library.

This is because this library is used for short-lived programs, e.g. cli tools
at beginning. It's very convenient to process remain arguments if we remove
parsed command-line arguments,  e.g. `<comamnd> [-[s]|--switch]... arguments`.

If you want keep original `argc/argv`, you can make a copy, then pass them to
`argparse_parse`, e.g.
