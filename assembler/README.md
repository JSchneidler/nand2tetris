# Hack Assembler

Project 6 of Nand to Tetris.

This program compiles a file containing Hack assembly code into Hack machine code.

The hack assembly language and machine language specifications are linked below:  
[Hack assembly language](https://docs.wixstatic.com/ugd/44046b_b73759b866b249a0b3a715bf5a18f668.pdf)  
[Hack machine language](https://docs.wixstatic.com/ugd/44046b_d70026d8c1424487a451eaba3e372132.pdf)

# Build

1. `make`

# Usage

`assembler.exe input_file`  
input_file - Path to the input file

The program generates an output file with a `.hack` extension and a basename equal to the input files.

# Architecture

The program consists of two classes used by main:  
`Parser` - Reads through each instruction in the input file, parsing it into fields  
`SymbolTable` - Used to manage labels in the input file and convert them to their respective addresses

The main function starts by iterating through all the `Parser`'s instructions, only looking for label declarations, and adds them to the `SymbolTable` with their corresponding address.

The parser is then reset and another pass is made. This time, everything else is parsed. Labels are converted to their respective addresses in the `SymbolTable` before the entire instruction is then converted to a 16-bit binary Hack machine instruction and output.
