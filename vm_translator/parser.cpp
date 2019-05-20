#include "parser.hpp"
#include <iostream>
#include <stdexcept>

static std::string trimWhitespace(const std::string& string);
static std::string stripComment(const std::string& string);
static bool isArithmeticInstruction(const std::string& string);
static Parser::SEGMENTS parseSegmentType(const std::string& string);

Parser::Parser(std::string inputFilename)
    : inputFile{inputFilename},
      inputFileBasename{
          inputFilename.substr(0, inputFilename.find_first_of('.'))},
      _moreInstructions{true},
      currentInstruction{},
      rawInstruction{""} {
  if (!inputFile.good() || inputFile.bad() || inputFile.fail() ||
      !inputFile.is_open())
    throw std::runtime_error("Invalid input file");
  advanceInstruction();
}

Parser::~Parser() { inputFile.close(); }

bool Parser::moreInstructions() const { return _moreInstructions; }

const Parser::Instruction& Parser::getCurrentInstruction() const {
  return currentInstruction;
}

void Parser::reset() {
  // Set file back to beginning
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);

  // Reset variables and re-initialize parser
  _moreInstructions = true;
  advanceInstruction();
}

void Parser::advanceInstruction() {
  std::string line;
  while (std::getline(inputFile, line)) {
    line = trimWhitespace(stripComment(line));
    if (line == "") continue;
    rawInstruction = line;
    parseInstruction(line);
    return;
  }
  _moreInstructions = false;
}

std::string Parser::getRawInstruction() { return rawInstruction; }

void Parser::parseInstruction(const std::string& line) {
  // Label instruction
  if (line.substr(0, 5) == "label") {
    currentInstruction.type = LABEL_INSTRUCTION;
    currentInstruction.symbol = line.substr(6);
    return;
  }

  // Push instruction
  if (line.substr(0, 4) == "push") {
    currentInstruction.type = PUSH_INSTRUCTION;
    size_t splitIndex = line.find_last_of(" ");
    currentInstruction.indexOrConstant = std::stoi(line.substr(splitIndex));
    currentInstruction.segment =
        parseSegmentType(line.substr(5, splitIndex - 5));
    return;
  }

  // Pop instruction
  if (line.substr(0, 3) == "pop") {
    currentInstruction.type = POP_INSTRUCTION;
    size_t splitIndex = line.find_last_of(" ");
    currentInstruction.indexOrConstant = std::stoi(line.substr(splitIndex));
    currentInstruction.segment =
        parseSegmentType(line.substr(4, splitIndex - 4));
    return;
  }

  // Arithmetic/Logical instruction
  if (isArithmeticInstruction(line)) {
    currentInstruction.type = ARITHMETIC_INSTRUCTION;
    currentInstruction.op = line;
    return;
  }

  // Conditional goto instruction
  if (line.substr(0, 7) == "if-goto") {
    currentInstruction.type = IF_INSTRUCTION;
    currentInstruction.symbol = line.substr(8);
    return;
  }

  // Goto instruction
  if (line.substr(0, 4) == "goto") {
    currentInstruction.type = GOTO_INSTRUCTION;
    currentInstruction.symbol = line.substr(5);
    return;
  }

  // Function declaration instruction
  if (line.substr(0, 8) == "function") {
    currentInstruction.type = FN_DECL_INSTRUCTION;
    size_t splitIndex = line.find_last_of(" ");
    currentInstruction.indexOrConstant = std::stoi(line.substr(splitIndex));
    currentInstruction.symbol = line.substr(9, splitIndex - 9);
    return;
  }

  // Function call instruction
  if (line.substr(0, 4) == "call") {
    currentInstruction.type = CALL_INSTRUCTION;
    size_t splitIndex = line.find_last_of(" ");
    currentInstruction.indexOrConstant = std::stoi(line.substr(splitIndex));
    currentInstruction.symbol = line.substr(5, splitIndex - 5);
    return;
  }

  // Function return instruction
  if (line.substr(0, 6) == "return") {
    currentInstruction.type = RETURN_INSTRUCTION;
    return;
  }

  // Unknown instruction
  currentInstruction.type = NONE_INSTRUCTION;
}

bool operator==(const Parser::Instruction& left,
                const Parser::Instruction& right) {
  if (left.type != right.type) return false;
  if (left.segment != right.segment) return false;
  if (left.indexOrConstant != right.indexOrConstant) return false;
  if (left.op != right.op) return false;
  if (left.symbol != right.symbol) return false;
  return true;
}
bool operator!=(const Parser::Instruction& left,
                const Parser::Instruction& right) {
  return !(left == right);
}

static std::string trimWhitespace(const std::string& string) {
  if (string == "") return string;

  size_t stringBegin = string.find_first_not_of(" ");
  size_t stringEnd = string.find_last_not_of(" ") + 1;

  return string.substr(stringBegin, stringEnd - stringBegin);
}

static std::string stripComment(const std::string& string) {
  if (string == "") return string;
  size_t commentBegin = string.find_first_of("//");
  return string.substr(0, commentBegin);
}

static bool isArithmeticInstruction(const std::string& string) {
  return (string == "add" || string == "sub" || string == "neg" ||
          string == "eq" || string == "gt" || string == "lt" ||
          string == "and" || string == "or" || string == "not");
}

static Parser::SEGMENTS parseSegmentType(const std::string& string) {
  if (string == "constant") return Parser::CONSTANT_SEGMENT;
  if (string == "local") return Parser::LOCAL_SEGMENT;
  if (string == "argument") return Parser::ARGUMENT_SEGMENT;
  if (string == "this") return Parser::THIS_SEGMENT;
  if (string == "that") return Parser::THAT_SEGMENT;
  if (string == "pointer") return Parser::POINTER_SEGMENT;
  if (string == "temp") return Parser::TEMP_SEGMENT;
  if (string == "static") return Parser::STATIC_SEGMENT;
  return Parser::NONE_SEGMENT;
}