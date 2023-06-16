# Jack Compiler

This program compiles a .jack file or directory containing .jack files into Hack virtual machine code.

## Prerequisites
* [Poetry](https://python-poetry.org)

## Build

1. `poetry build`


## Test
1. `poetry run pytest`

## Usage

`python -m jack_compiler.cli <fileOrDir>`  
fileOrDir - Path to a .jack file or directory containing one or more .jack files

Generates one `.vm` file per class in Hack virtual machine language.

## Architecture

TODO
