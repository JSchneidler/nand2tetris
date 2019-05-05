#include "translator.hpp"

#include <iostream>

// Helper functions for generating ASM instructions
static std::string makeLine(const std::string &string);
static std::string selectStackPointer();
static std::string incrementStackPointer();
static std::string decrementStackPointer();
static std::string selectStack();
static std::string addLabel(const std::string &string);
static std::string equalityCheck(
    const Translator::EQUALITY_CHECK_TYPE checkType,
    const std::string &symbolName);

static std::string selectRegister(const std::string &string);
static std::string selectRegister(const short r);

Translator::Translator() : symbolCounter{0}, symbolPrefix{""} {}

std::string Translator::initializeStackPointer(const short stackAddress) {
  // Set stack pointer register to the stack's base address
  std::string instruction{selectRegister(stackAddress)};
  instruction += makeLine("D=A");
  instruction += selectStackPointer();
  instruction += makeLine("M=D");

  return instruction;
}

std::string Translator::generatePushInstruction(
    const std::string &segmentStackPointer, const short index) {
  // Store segment index in D register
  std::string instruction{selectRegister(index)};
  instruction += makeLine("D=A");
  instruction += selectRegister(segmentStackPointer);
  // Select segment + index
  instruction += makeLine("A=D+M");
  instruction += makeLine("D=M");
  // Push D reg on stack
  instruction += selectStack();
  instruction += makeLine("M=D");
  instruction += incrementStackPointer();

  return instruction;
}

std::string Translator::generatePushConstantInstruction(const short value) {
  // Store value in D register
  std::string instruction{selectRegister(value)};
  instruction += makeLine("D=A");

  // Push D reg on stack
  instruction += selectStack();
  instruction += makeLine("M=D");
  instruction += incrementStackPointer();

  return instruction;
}

std::string Translator::generatePushTempInstruction(const short index) {
  std::string instruction{selectRegister(5 + index)};
  instruction += makeLine("D=M");
  // Push D reg on stack
  instruction += selectStack();
  instruction += makeLine("M=D");
  instruction += incrementStackPointer();

  return instruction;
}

std::string Translator::generatePushPointerInstruction(const short index) {
  std::string instruction{selectRegister(3 + index)};
  instruction += makeLine("D=M");
  // Push D reg on stack
  instruction += selectStack();
  instruction += makeLine("M=D");
  instruction += incrementStackPointer();

  return instruction;
}

std::string Translator::generatePushStaticInstruction(const short index) {
  std::string instruction{selectRegister(generateSymbol(index))};
  instruction += makeLine("D=M");
  // Push D reg on stack
  instruction += selectStack();
  instruction += makeLine("M=D");
  instruction += incrementStackPointer();

  return instruction;
}

std::string Translator::generatePopInstruction(
    const std::string &segmentStackPointer, const short index) {
  // Store segment address + index in R13
  std::string instruction{selectRegister(index)};
  instruction += makeLine("D=A");
  instruction += selectRegister(segmentStackPointer);
  instruction += makeLine("D=D+M");
  instruction += selectRegister("R13");
  instruction += makeLine("M=D");
  // Pop value off stack into address at R13
  instruction += decrementStackPointer();
  instruction += selectStack();
  instruction += makeLine("D=M");
  instruction += selectRegister("R13");
  instruction += makeLine("A=M");
  instruction += makeLine("M=D");

  return instruction;
}

std::string Translator::generatePopTempInstruction(const short index) {
  std::string instruction{decrementStackPointer()};
  instruction += selectStack();
  instruction += makeLine("D=M");
  instruction += selectRegister(5 + index);
  instruction += makeLine("M=D");

  return instruction;
}

std::string Translator::generatePopPointerInstruction(const short index) {
  // Store pointer address in R13
  std::string instruction{selectRegister(3 + index)};
  instruction += makeLine("D=A");
  instruction += selectRegister("R13");
  instruction += makeLine("M=D");
  // Pop value off stack into address at R13
  instruction += decrementStackPointer();
  instruction += selectStack();
  instruction += makeLine("D=M");
  instruction += selectRegister("R13");
  instruction += makeLine("A=M");
  instruction += makeLine("M=D");

  return instruction;
}

std::string Translator::generatePopStaticInstruction(const short index) {
  // Store pointer address in R13
  std::string instruction{selectRegister(generateSymbol(index))};
  instruction += makeLine("D=A");
  instruction += selectRegister("R13");
  instruction += makeLine("M=D");
  // Pop value off stack into address at R13
  instruction += decrementStackPointer();
  instruction += selectStack();
  instruction += makeLine("D=M");
  instruction += selectRegister("R13");
  instruction += makeLine("A=M");
  instruction += makeLine("M=D");

  return instruction;
}

std::string Translator::generateArithmeticInstruction(const std::string &op) {
  // Start by popping first value (Y) off stack
  std::string instruction{decrementStackPointer()};

  // Unary operations
  if (op == "neg" || op == "not") {
    // Select top of stack
    instruction += selectStack();

    // Perform operation
    if (op == "neg")
      instruction += makeLine("M=-M");
    else
      instruction += makeLine("M=!M");
  }

  // Binary operations
  else {
    // Pop first value (Y) off stack into D register
    instruction += selectStack();
    instruction += makeLine("D=M");

    // Pop second value (X) off stack
    instruction += decrementStackPointer();
    instruction += selectStack();

    // Perform operation and store back on stack
    if (op == "add")
      instruction += makeLine("M=D+M");
    else if (op == "sub")
      instruction += makeLine("M=M-D");
    else if (op == "and")
      instruction += makeLine("M=M&D");
    else if (op == "or")
      instruction += makeLine("M=M|D");
    else if (op == "eq")
      instruction += equalityCheck(Translator::EQ_CHECK, getNextSymbolName());
    else if (op == "gt")
      instruction += equalityCheck(Translator::GT_CHECK, getNextSymbolName());
    else if (op == "lt")
      instruction += equalityCheck(Translator::LT_CHECK, getNextSymbolName());
  }

  instruction += incrementStackPointer();

  return instruction;
}

void Translator::restartSymbolCounter() { symbolCounter = 0; }

void Translator::setSymbolPrefix(const std::string &prefix) {
  symbolPrefix = prefix;
}

std::string Translator::getNextSymbolName() {
  std::string symbolName{"_" + symbolPrefix + "." +
                         std::to_string(symbolCounter)};
  ++symbolCounter;
  return symbolName;
}

std::string Translator::generateSymbol(const short index) {
  return symbolPrefix + "." + std::to_string(index);
}

static std::string makeLine(const std::string &string) { return string + "\n"; }

static std::string selectStackPointer() { return selectRegister("SP"); }

static std::string incrementStackPointer() {
  return selectStackPointer() + makeLine("M=M+1");
}
static std::string decrementStackPointer() {
  return selectStackPointer() + makeLine("M=M-1");
}

static std::string selectStack() {
  return selectStackPointer() + makeLine("A=M");
}

static std::string addLabel(const std::string &string) {
  return makeLine("(" + string + ")");
}

// This function assumes the two values were popped off the stack into D and M
// already
static std::string equalityCheck(
    const Translator::EQUALITY_CHECK_TYPE checkType,
    const std::string &symbolName) {
  std::string jumpType;
  if (checkType == Translator::EQ_CHECK) jumpType = "JNE";
  if (checkType == Translator::GT_CHECK) jumpType = "JGE";
  if (checkType == Translator::LT_CHECK) jumpType = "JLE";

  std::string instruction;
  instruction += makeLine("D=D-M");
  instruction += makeLine("M=0");  // Assume false
  instruction += selectRegister(symbolName);
  instruction += makeLine("D;" + jumpType);  // Compare, Goto continue if passes
  instruction += selectStack();
  instruction += makeLine("M=-1");      // Set true
  instruction += addLabel(symbolName);  // Continue

  return instruction;
}

static std::string selectRegister(const std::string &string) {
  return makeLine("@" + string);
}

static std::string selectRegister(const short r) {
  return selectRegister(std::to_string(r));
}