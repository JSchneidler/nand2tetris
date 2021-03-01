# Jack Compiler
This program compiles a .jack file or directory containing .jack files into Hack virtual machine code.

## Build
1. `mkdir build && cd build`
2. `cmake --configure ..`
3. `cmake --build .`

## Usage
`./build/src/JackC input_path`  
input_path - Path to a .jack file or directory containing one or more .jack files

The program generates a `.vm` file in Hack virtual machine language.

## Architecture
Main: The JackC program that configures and executes the rest of the Jack compiler  
1. Lexer: Tokenizes each Jack file  
2. Parser (WIP): Generates a parse tree from a tokenized Jack file  
3. CodeGenerator (WIP): Generates Hack VM code from a parse tree