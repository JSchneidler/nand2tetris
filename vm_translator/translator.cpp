#include "translator.hpp"
#include <iostream>

Translator::Translator()
    : symbolCounter{0}, instructionCount{0}, symbolPrefix{""} {}

void Translator::restartSymbolCounter() { symbolCounter = 0; }

void Translator::setSymbolPrefix(const std::string &prefix) {
  symbolPrefix = prefix;
}

// Initializes stack pointer to a passed address
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

std::string Translator::generatePushZeroToStackInstruction() {
  std::string instruction{selectStack()};
  instruction += makeLine("M=0");
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

std::string Translator::generateLabelInstruction(const std::string &symbol) {
  return addLabel(symbol);
}

std::string Translator::generateConditionalGotoInstruction(
    const std::string &symbol) {
  // Pop value off stack into D register
  std::string instruction{decrementStackPointer()};
  instruction += selectStack();
  instruction += makeLine("D=M");
  // Select jump location
  instruction += selectRegister(symbol);
  // Jump if value is not equal to 0
  instruction += makeLine("D;JNE");

  return instruction;
}

std::string Translator::generateGotoInstruction(const std::string &symbol) {
  return selectRegister(symbol) + makeLine("0;JMP");
}

std::string Translator::generateFnDeclInstruction(const std::string &symbol,
                                                  const short localVars) {
  std::string instruction{addLabel(symbol)};
  for (short i = 0; i < localVars; i++) {
    instruction += generatePushZeroToStackInstruction();
  }

  return instruction;
}

std::string Translator::generateCallInstruction(const std::string &symbol,
                                                const short pushedVars = 0) {
  std::string returnAddrSymbolName = getNextSymbolName();

  // Push return address
  std::string instruction{pushRegisterToStack(returnAddrSymbolName)};

  // Push LCL
  instruction += pushRegisterToStack("LCL");

  // Push ARG
  instruction += pushRegisterToStack("ARG");

  // Push THIS
  instruction += pushRegisterToStack("THIS");

  // Push THAT
  instruction += pushRegisterToStack("THAT");

  // Reposition ARG (ARG = SP-n-5)
  instruction += selectStackPointer();
  instruction += makeLine("D=M");
  instruction += selectRegister(pushedVars);
  instruction += makeLine("D=D-A");
  instruction += selectRegister(5);
  instruction += makeLine("D=D-A");
  instruction += selectRegister("ARG");
  instruction += makeLine("M=D");

  // Reposition LCL (LCL = SP)
  instruction += selectStackPointer();
  instruction += makeLine("D=M");
  instruction += selectRegister("LCL");
  instruction += makeLine("M=D");

  // Goto f
  instruction += generateGotoInstruction(symbol);

  // Label for return address
  instruction += addLabel(returnAddrSymbolName);

  return instruction;
}

std::string Translator::generateReturnInstruction() {
  // Temporarily store top of frame in R13
  std::string instruction{selectRegister("LCL")};
  instruction += makeLine("D=M");
  instruction += selectRegister("R13");
  instruction += makeLine("M=D");

  // TODO FIX
  // Temporarily store return address in R14
  instruction += selectRegister(5);
  instruction += makeLine("D=D-A");
  instruction += makeLine("A=D");
  instruction += makeLine("D=M");
  instruction += selectRegister("R14");
  instruction += makeLine("M=D");

  // Swap arg with return value
  instruction += decrementStackPointer();
  instruction += selectStack();
  instruction += makeLine("D=M");
  instruction += selectRegister("ARG");
  instruction += makeLine("A=M");
  instruction += makeLine("M=D");

  // Restore SP  = *(ARG + 1)
  instruction += selectRegister("ARG");
  instruction += makeLine("D=M+1");
  instruction += selectRegister("SP");
  instruction += makeLine("M=D");

  // Restore THAT  = *(FRAME - 1)
  instruction += selectRegister("R13");
  instruction += makeLine("D=M-1");
  instruction += makeLine("A=D");
  instruction += makeLine("D=M");
  instruction += selectRegister("THAT");
  instruction += makeLine("M=D");

  // Restore THIS  = *(FRAME - 2)
  instruction += selectRegister(2);
  instruction += makeLine("D=A");
  instruction += selectRegister("R13");
  instruction += makeLine("D=M-D");
  instruction += makeLine("A=D");
  instruction += makeLine("D=M");
  instruction += selectRegister("THIS");
  instruction += makeLine("M=D");

  // Restore ARG  = *(FRAME - 3)
  instruction += selectRegister(3);
  instruction += makeLine("D=A");
  instruction += selectRegister("R13");
  instruction += makeLine("D=M-D");
  instruction += makeLine("A=D");
  instruction += makeLine("D=M");
  instruction += selectRegister("ARG");
  instruction += makeLine("M=D");

  // Restore LCL  = *(FRAME - 4)
  instruction += selectRegister(4);
  instruction += makeLine("D=A");
  instruction += selectRegister("R13");
  instruction += makeLine("D=M-D");
  instruction += makeLine("A=D");
  instruction += makeLine("D=M");
  instruction += selectRegister("LCL");
  instruction += makeLine("M=D");

  // Goto return address
  instruction += selectRegister("R14");
  instruction += makeLine("A=M");
  instruction += makeLine("0;JMP");

  return instruction;
}

short Translator::getCurrentInstructionNumber() { return instructionCount; }

// Appends a newline to the end of string. That's it.
std::string Translator::makeLine(const std::string &string,
                                 bool dontIncreaseInstructionNumber) {
  if (!dontIncreaseInstructionNumber) ++instructionCount;
  return string + "\n";
}

// Generates a unique symbol name
std::string Translator::getNextSymbolName() {
  std::string symbolName{"_" + symbolPrefix + "." +
                         std::to_string(symbolCounter)};
  ++symbolCounter;
  return symbolName;
}

// Generates a symbol based on set prefix and passed index
std::string Translator::generateSymbol(const short index) {
  return symbolPrefix + "." + std::to_string(index);
}

// Selects the SP register
std::string Translator::selectStackPointer() { return selectRegister("SP"); }

// Selects the SP register and increases it by 1
std::string Translator::incrementStackPointer() {
  return selectStackPointer() + makeLine("M=M+1");
}

// Selects the SP register and decreases it by 1
std::string Translator::decrementStackPointer() {
  return selectStackPointer() + makeLine("M=M-1");
}

// Selects the register at the top of the stack
std::string Translator::selectStack() {
  return selectStackPointer() + makeLine("A=M");
}

// Generates a label
std::string Translator::addLabel(const std::string &string) {
  return makeLine("(" + string + ")", true);
}

// This function assumes one value was popped off the stack into D and M is the
// new top of the stack (second value).
std::string Translator::equalityCheck(
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

// Selects a register denoted by the string
std::string Translator::selectRegister(const std::string &string) {
  return makeLine("@" + string);
}

// Selects a register number
std::string Translator::selectRegister(const short r) {
  return selectRegister(std::to_string(r));
}

std::string Translator::pushRegisterToStack(const std::string &string) {
  std::string instruction{selectRegister(string)};
  instruction += makeLine("D=M");
  instruction += selectStack();
  instruction += makeLine("M=D");
  instruction += incrementStackPointer();

  return instruction;
}