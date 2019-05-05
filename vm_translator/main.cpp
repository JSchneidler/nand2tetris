#include <iostream>
#include <stdexcept>
#include <string>
#include "parser.hpp"
#include "translator.hpp"

const short STACK_BASE_ADDR = 0x100;

static std::string getOutputFilename(const std::string &string);
static std::string getFileBasename(const std::string &string);

int main(int, const char *argv[]) {
  if (!argv[1]) throw std::invalid_argument("No input file given");

  const std::string inputFilename{argv[1]};
  std::ofstream outputFile{getOutputFilename(inputFilename)};

  Parser parser{inputFilename};
  Translator translator{};

  // Set prefix to use for symbols in jump statements (for use later with more
  // files)
  translator.setSymbolPrefix(getFileBasename(inputFilename));

  // Set stack pointer to base address of stack
  outputFile << Translator::initializeStackPointer(STACK_BASE_ADDR);

  // Iterate through instructions
  for (; parser.moreInstructions(); parser.advanceInstruction()) {
    Parser::Instruction currentInstruction{parser.getCurrentInstruction()};

    // Push instruction
    if (currentInstruction.type == Parser::PUSH_INSTRUCTION) {
      std::cout << "INSTRUCTION: push ";
      if (currentInstruction.segment == Parser::CONSTANT_SEGMENT) {
        std::string instruction = Translator::generatePushConstantInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "constant " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::LOCAL_SEGMENT) {
        std::string instruction = Translator::generatePushInstruction(
            "LCL", currentInstruction.indexOrConstant);
        std::cout << "local " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::ARGUMENT_SEGMENT) {
        std::string instruction = Translator::generatePushInstruction(
            "ARG", currentInstruction.indexOrConstant);
        std::cout << "argument " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::THIS_SEGMENT) {
        std::string instruction = Translator::generatePushInstruction(
            "THIS", currentInstruction.indexOrConstant);
        std::cout << "this " << currentInstruction.indexOrConstant << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::THAT_SEGMENT) {
        std::string instruction = Translator::generatePushInstruction(
            "THAT", currentInstruction.indexOrConstant);
        std::cout << "that " << currentInstruction.indexOrConstant << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::POINTER_SEGMENT) {
        std::string instruction = Translator::generatePushPointerInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "pointer " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::TEMP_SEGMENT) {
        std::string instruction = Translator::generatePushTempInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "temp " << currentInstruction.indexOrConstant << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::STATIC_SEGMENT) {
        std::string instruction = translator.generatePushStaticInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "static " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      }
    }
    // Pop Instruction
    else if (currentInstruction.type == Parser::POP_INSTRUCTION) {
      std::cout << "INSTRUCTION: pop ";
      if (currentInstruction.segment == Parser::LOCAL_SEGMENT) {
        std::string instruction = Translator::generatePopInstruction(
            "LCL", currentInstruction.indexOrConstant);
        std::cout << "local " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::ARGUMENT_SEGMENT) {
        std::string instruction = Translator::generatePopInstruction(
            "ARG", currentInstruction.indexOrConstant);
        std::cout << "argument " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::THIS_SEGMENT) {
        std::string instruction = Translator::generatePopInstruction(
            "THIS", currentInstruction.indexOrConstant);
        std::cout << "this " << currentInstruction.indexOrConstant << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::THAT_SEGMENT) {
        std::string instruction = Translator::generatePopInstruction(
            "THAT", currentInstruction.indexOrConstant);
        std::cout << "that " << currentInstruction.indexOrConstant << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::POINTER_SEGMENT) {
        std::string instruction = Translator::generatePopPointerInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "pointer " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::TEMP_SEGMENT) {
        std::string instruction = Translator::generatePopTempInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "temp " << currentInstruction.indexOrConstant << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      } else if (currentInstruction.segment == Parser::STATIC_SEGMENT) {
        std::string instruction = translator.generatePopStaticInstruction(
            currentInstruction.indexOrConstant);
        std::cout << "static " << currentInstruction.indexOrConstant
                  << std::endl;
        std::cout << instruction << std::endl;
        outputFile << instruction;
      }
    }

    // Arithmetic/Logical Instruction
    else if (currentInstruction.type == Parser::ARITHMETIC_INSTRUCTION) {
      std::string instruction =
          translator.generateArithmeticInstruction(currentInstruction.op);
      outputFile << instruction;
    }
  }

  outputFile.close();

  return 0;
}

static std::string getOutputFilename(const std::string &string) {
  return string.substr(0, string.find_first_of('.')) + ".asm";
}

static std::string getFileBasename(const std::string &string) {
  return string.substr(0, string.find_first_of('.'));
}