#include "parser.hpp"
#include <iostream>

Parser::Parser(const std::string& inputFilename)
    : inputFile(std::ifstream(inputFilename)),
      currentInstructionNumber(-1),
      currentInstruction(""),
      currentInstructionType(NONE),
      currentInstructionSymbol(""),
      currentInstructionCompField(""),
      currentInstructionDestField(""),
      currentInstructionJmpField("") {}

Parser::~Parser() { inputFile.close(); }

int Parser::getCurrentInstructionNumber() const {
  return currentInstructionNumber;
}

std::string Parser::getCurrentInstructionSymbol() const {
  return currentInstructionSymbol;
}

std::string Parser::getCurrentInstructionCompField() const {
  return currentInstructionCompField;
}

std::string Parser::getCurrentInstructionDestField() const {
  return currentInstructionDestField;
}

std::string Parser::getCurrentInstructionJmpField() const {
  return currentInstructionJmpField;
}

void Parser::reset() {
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);
  currentInstructionNumber = -1;
}

// Advances parser state to the next instruction in the file.
// Returns true if a command was found and false otherwise.
bool Parser::advanceToNextInstruction() {
  // Read lines until we find a valid command
  while (std::getline(inputFile, currentInstruction)) {
    currentInstruction = trim(stripComment(currentInstruction));

    if (currentInstruction != "") {
      // Parse out instruction fields and any symbols
      parseCurrentInstruction();
      // Only increase instruction number if it is an A or C command, not an
      // internal L command.
      if (!currentInstructionIsType(L_INSTRUCTION)) ++currentInstructionNumber;
      return true;
    }
  }

  return false;
}

bool Parser::currentInstructionIsType(instructionType type) const {
  return currentInstructionType == type;
}

void Parser::parseCurrentInstruction() {
  if (currentInstruction[0] == '(') {
    currentInstructionType = L_INSTRUCTION;
    currentInstructionSymbol =
        currentInstruction.substr(1, currentInstruction.size() - 2);
  } else if (currentInstruction[0] == '@') {
    currentInstructionType = A_INSTRUCTION;
    currentInstructionSymbol = currentInstruction.substr(1);
  } else {
    currentInstructionType = C_INSTRUCTION;
    currentInstructionSymbol = "";

    size_t destFieldSplitter{currentInstruction.find_first_of('=')};
    size_t jmpFieldSplitter{currentInstruction.find_first_of(';')};
    size_t compStart{0};
    size_t compEnd{currentInstruction.length()};

    if (destFieldSplitter != std::string::npos) {
      currentInstructionDestField =
          destMap.at(currentInstruction.substr(0, destFieldSplitter));
      compStart = destFieldSplitter + 1;
    } else
      currentInstructionDestField = "000";

    if (jmpFieldSplitter != std::string::npos) {
      currentInstructionJmpField =
          jmpMap.at(currentInstruction.substr(jmpFieldSplitter + 1));
      compEnd = jmpFieldSplitter;
    } else
      currentInstructionJmpField = "000";

    currentInstructionCompField =
        compMap.at(currentInstruction.substr(compStart, compEnd));
  }
}

std::string trim(const std::string& string) {
  if (string == "") return string;

  size_t start{string.find_first_not_of(' ')};
  size_t end{string.find_last_not_of(' ')};
  return string.substr(start, (end - start + 1));
}

std::string stripComment(const std::string& string) {
  if (string == "") return string;

  size_t i{string.find("//")};
  if (i != std::string::npos) {
    return string.substr(0, i);
  }

  return string;
}