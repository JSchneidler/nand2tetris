#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "parser.hpp"
#include "symboltable.hpp"

const int VARIABLE_STACK_BASE_ADDRESS{0x10};

int main(int, const char *argv[])
{
  if (!argv[1])
    throw std::invalid_argument("No input file received");

  int nextVariableStackAddress{VARIABLE_STACK_BASE_ADDRESS};

  const std::filesystem::path inputPath{argv[1]};

  SymbolTable symbolTable{};
  Parser parser{inputPath};

  // First pass
  for (; parser.moreCommands(); parser.advanceCommand())
  {
    if (parser.commandIsType(Parser::L_COMMAND) &&
        !symbolTable.contains(parser.getCommandSymbol()))
      symbolTable.addSymbol(parser.getCommandSymbol(),
                            parser.getInstructionNumber() + 1);
  }

  std::filesystem::path outputPath{inputPath};
  outputPath.replace_extension(".hack");
  std::ofstream outputFile{outputPath};

  // Second pass
  std::string machineInstruction;
  for (parser.reset(); parser.moreCommands(); parser.advanceCommand())
  {
    if (parser.commandIsType(Parser::A_INSTRUCTION))
    {
      std::string symbol = parser.getCommandSymbol();

      if (!symbolTable.contains(symbol))
      {
        symbolTable.addSymbol(symbol, nextVariableStackAddress);
        ++nextVariableStackAddress;
      }

      int symbolValue;
      if (!isdigit(symbol[0]))
        symbolValue = symbolTable.getSymbolValue(symbol);
      else
        symbolValue = std::stoi(symbol);

      machineInstruction = "0" + std::bitset<15>(symbolValue).to_string();

      outputFile << machineInstruction << std::endl;
    }
    else if (parser.commandIsType(Parser::C_INSTRUCTION))
    {
      machineInstruction = "111" + parser.getInstructionCompField() +
                           parser.getInstructionDestField() +
                           parser.getInstructionJmpField();

      outputFile << machineInstruction << std::endl;
    }
  }

  outputFile.close();

  return 0;
}
