#include "parser.hpp"
#include <iostream>

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
      nextSymbolId{0} {
  if (!inputFile.good() || inputFile.bad() || inputFile.fail() ||
      !inputFile.is_open())
    throw std::invalid_argument("Invalid input file");
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
    parseInstruction(line);
    return;
  }
  _moreInstructions = false;
}

std::string Parser::getNextSymbolName() {
  std::string symbolName = inputFileBasename + std::to_string(nextSymbolId);
  ++nextSymbolId;
  return symbolName;
}

void Parser::parseInstruction(const std::string& line) {
  // Label instruction
  if (line.substr(0, 5) == "label") {
    currentInstruction.type = LABEL_INSTRUCTION;
  }

  // Push instruction
  else if (line.substr(0, 4) == "push") {
    currentInstruction.type = PUSH_INSTRUCTION;
    size_t splitIndex = line.find_last_of(" ");
    currentInstruction.indexOrConstant = std::stoi(line.substr(splitIndex));
    currentInstruction.segment =
        parseSegmentType(line.substr(5, splitIndex - 5));
  }

  // Pop instruction
  else if (line.substr(0, 3) == "pop") {
    currentInstruction.type = POP_INSTRUCTION;
    size_t splitIndex = line.find_last_of(" ");
    currentInstruction.indexOrConstant = std::stoi(line.substr(splitIndex));
    currentInstruction.segment =
        parseSegmentType(line.substr(4, splitIndex - 4));
  }

  // Arithmetic/Logical instruction
  else if (isArithmeticInstruction(line)) {
    currentInstruction.type = ARITHMETIC_INSTRUCTION;
    currentInstruction.op = line;
  }

  // Unknown instruction
  else
    currentInstruction.type = NONE;
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
}