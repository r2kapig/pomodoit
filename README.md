
# argparse

[![Build Status](https://github.com/cofyc/argparse/actions/workflows/ci.yaml/badge.svg?branch=master)](https://github.com/cofyc/argparse/actions/workflows/ci.yaml)

argparse - A command line arguments parsing library in C (compatible with C++).

## Description

This module is inspired by parse-options.c (git) and python's argparse
module.

Arguments parsing is common task in cli program, but traditional `getopt`
libraries are not easy to use. This library provides high-level arguments
parsing solutions.

The program defines what arguments it requires, and `argparse` will figure
out how to parse those out of `argc` and `argv`, it also automatically
generates help and usage messages and issues errors when users give the