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
  outputFile << translator.initializeStackPointer(STACK_BASE_ADDR);
  // outputFile << translator.generateCallInstruction("Sys.init");

  // Iterate through instructions
  for (; parser.moreInstructions(); parser.advanceInstruction()) {
    Parser::Instruction currentInstruction{parser.getCurrentInstruction()};

    std::cout << "INSTRUCTION: " << parser.getRawInstruction() << std::endl;
    std::cout << "Instruction number: "
              << translator.getCurrentInstructionNumber() << std::endl;

    // Push instruction
    if (currentInstruction.type == Parser::PUSH_INSTRUCTION) {
      if (currentInstruction.segment == Parser::CONSTANT_SEGMENT) {
        std::string instruction = translator.generatePushConstantInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::LOCAL_SEGMENT) {
        std::string instruction = translator.generatePushInstruction(
            "LCL", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::ARGUMENT_SEGMENT) {
        std::string instruction = translator.generatePushInstruction(
            "ARG", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::THIS_SEGMENT) {
        std::string instruction = translator.generatePushInstruction(
            "THIS", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::THAT_SEGMENT) {
        std::string instruction = translator.generatePushInstruction(
            "THAT", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::POINTER_SEGMENT) {
        std::string instruction = translator.generatePushPointerInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::TEMP_SEGMENT) {
        std::string instruction = translator.generatePushTempInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::STATIC_SEGMENT) {
        std::string instruction = translator.generatePushStaticInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }
    }

    // Pop Instruction
    else if (currentInstruction.type == Parser::POP_INSTRUCTION) {
      if (currentInstruction.segment == Parser::LOCAL_SEGMENT) {
        std::string instruction = translator.generatePopInstruction(
            "LCL", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::ARGUMENT_SEGMENT) {
        std::string instruction = translator.generatePopInstruction(
            "ARG", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::THIS_SEGMENT) {
        std::string instruction = translator.generatePopInstruction(
            "THIS", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::THAT_SEGMENT) {
        std::string instruction = translator.generatePopInstruction(
            "THAT", currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::POINTER_SEGMENT) {
        std::string instruction = translator.generatePopPointerInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::TEMP_SEGMENT) {
        std::string instruction = translator.generatePopTempInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }

      else if (currentInstruction.segment == Parser::STATIC_SEGMENT) {
        std::string instruction = translator.generatePopStaticInstruction(
            currentInstruction.indexOrConstant);
        outputFile << instruction;

        std::cout << instruction << std::endl;
      }
    }

    // Arithmetic/Logical Instruction
    else if (currentInstruction.type == Parser::ARITHMETIC_INSTRUCTION) {
      std::string instruction =
          translator.generateArithmeticInstruction(currentInstruction.op);
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    // Label instruction
    else if (currentInstruction.type == Parser::LABEL_INSTRUCTION) {
      std::string instruction =
          translator.generateLabelInstruction(currentInstruction.symbol);
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    // Conditional jump instruction
    else if (currentInstruction.type == Parser::IF_INSTRUCTION) {
      std::string instruction = translator.generateConditionalGotoInstruction(
          currentInstruction.symbol);
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    // Goto instruction
    else if (currentInstruction.type == Parser::GOTO_INSTRUCTION) {
      std::string instruction =
          translator.generateGotoInstruction(currentInstruction.symbol);
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    // Function declaration instruction
    else if (currentInstruction.type == Parser::FN_DECL_INSTRUCTION) {
      std::string instruction = translator.generateFnDeclInstruction(
          currentInstruction.symbol, currentInstruction.indexOrConstant);
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    // Function call instruction
    else if (currentInstruction.type == Parser::CALL_INSTRUCTION) {
      std::string instruction = translator.generateCallInstruction(
          currentInstruction.symbol, currentInstruction.indexOrConstant);
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    // Function return instruction
    else if (currentInstruction.type == Parser::RETURN_INSTRUCTION) {
      std::string instruction = translator.generateReturnInstruction();
      outputFile << instruction;

      std::cout << instruction << std::endl;
    }

    else {
      std::cout << "UNKNOWN INSTRUCTION: " << parser.getRawInstruction()
                << std::endl;
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