# Hack VM Translator

This program compiles a file containing Hack virtual machine code into Hack assembly code.

The hack virtual machine specifications are linked below:  
[Part 1](http://www.cs.huji.ac.il/course/2002/nand2tet/docs/ch_7_vm_I.pdf)
[Part 2](http://www.cs.huji.ac.il/course/2002/nand2tet/docs/ch_8_vm_II.pdf)

## Build

1. `make`

## Usage

`vm_translator.exe input_file`  
input_file - Path to the input file

The program generates an output file with a `.asm` extension and a basename equal to the input file's.

## Architecture

The program consists of two classes used by main:  
`Parser` - Reads through each instruction in the input file, parsing it into fields  
`Translator` - Generates sequences of assembly commands for each virtual machine command

The main function starts by iterating through all the `Parser`'s instructions, only looking for label declarations, and adds them to the `SymbolTable` with their corresponding address.

The parser is then reset and another pass is made. This time, everything else is parsed. Labels are converted to their respective addresses in the `SymbolTable` before the entire instruction is then converted to a 16-bit binary Hack machine instruction and output.
