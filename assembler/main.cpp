#include <bitset>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "parser.hpp"
#include "symboltable.hpp"

static std::string generateOutputFilename(const std::string& inputFilename);
static std::string to15BitBinaryString(const int number);

int main(int, const char* argv[]) {
  if (!argv[1]) throw std::invalid_argument("No input file received");

  const int VARIABLE_STACK_BASE_ADDRESS{0x10};
  int nextVariableStackAddress{VARIABLE_STACK_BASE_ADDRESS};

  std::string inputFilename{argv[1]};
  std::string outputFilename{generateOutputFilename(inputFilename)};

  std::ofstream outputFile{outputFilename};

  SymbolTable symbolTable{};
  Parser parser{inputFilename};

  // First pass
  while (parser.advanceToNextInstruction()) {
    if (parser.currentInstructionIsType(Parser::L_INSTRUCTION) &&
        !symbolTable.contains(parser.getCurrentInstructionSymbol()))
      symbolTable.addSymbol(parser.getCurrentInstructionSymbol(),
                            parser.getCurrentInstructionNumber() + 1);
  }

  // Second pass
  parser.reset();

  std::string machineInstruction;
  while (parser.advanceToNextInstruction()) {
    if (parser.currentInstructionIsType(Parser::A_INSTRUCTION)) {
      if (!symbolTable.contains(parser.getCurrentInstructionSymbol())) {
        symbolTable.addSymbol(parser.getCurrentInstructionSymbol(),
                              nextVariableStackAddress);
        ++nextVariableStackAddress;
      }

      int symbolValue;
      if (!isdigit(parser.getCurrentInstructionSymbol()[0]))
        symbolValue =
            symbolTable.getSymbolValue(parser.getCurrentInstructionSymbol());
      else
        symbolValue = std::stoi(parser.getCurrentInstructionSymbol());

      machineInstruction = "0" + to15BitBinaryString(symbolValue);

      outputFile << machineInstruction << std::endl;
    } else if (parser.currentInstructionIsType(Parser::C_INSTRUCTION)) {
      machineInstruction = "111" + parser.getCurrentInstructionCompField() +
                           parser.getCurrentInstructionDestField() +
                           parser.getCurrentInstructionJmpField();

      outputFile << machineInstruction << std::endl;
    }
  }

  outputFile.close();

  return 0;
}

static std::string generateOutputFilename(const std::string& inputFilename) {
  size_t filenameEnd{inputFilename.find_first_of('.')};
  return inputFilename.substr(0, filenameEnd) + ".hack";
}

static std::string to15BitBinaryString(const int number) {
  return std::bitset<15>(number).to_string();
}