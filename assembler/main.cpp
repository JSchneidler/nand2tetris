#include <bitset>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "parser.hpp"
#include "symboltable.hpp"

static std::string generateOutputFilename(const std::string &inputFilename);
static std::string to15BitBinaryString(const int number);

const int VARIABLE_STACK_BASE_ADDRESS{0x10};

int main(int, const char *argv[])
{
  if (!argv[1])
    throw std::invalid_argument("No input file received");

  int nextVariableStackAddress{VARIABLE_STACK_BASE_ADDRESS};

  std::string inputFilename{argv[1]};
  std::string outputFilename{generateOutputFilename(inputFilename)};

  std::ofstream outputFile{outputFilename};

  SymbolTable symbolTable{};
  Parser parser{inputFilename};

  // First pass
  for (; parser.moreCommands(); parser.advanceCommand())
  {
    if (parser.commandIsType(Parser::L_COMMAND) &&
        !symbolTable.contains(parser.getCommandSymbol()))
      symbolTable.addSymbol(parser.getCommandSymbol(),
                            parser.getInstructionNumber() + 1);
  }

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

      machineInstruction = "0" + to15BitBinaryString(symbolValue);

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

static std::string generateOutputFilename(const std::string &inputFilename)
{
  size_t filenameEnd{inputFilename.find_first_of('.')};
  return inputFilename.substr(0, filenameEnd) + ".hack";
}

static std::string to15BitBinaryString(const int number)
{
  return std::bitset<15>(number).to_string();
}