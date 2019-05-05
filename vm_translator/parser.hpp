#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>

class Parser {
 public:
  enum INSTRUCTION_TYPES {
    LABEL_INSTRUCTION,
    PUSH_INSTRUCTION,
    POP_INSTRUCTION,
    ARITHMETIC_INSTRUCTION,
    NONE
  };
  enum SEGMENTS {
    CONSTANT_SEGMENT,
    LOCAL_SEGMENT,
    ARGUMENT_SEGMENT,
    THIS_SEGMENT,
    THAT_SEGMENT,
    POINTER_SEGMENT,
    TEMP_SEGMENT,
    STATIC_SEGMENT
  };
  struct Instruction {
    INSTRUCTION_TYPES type;
    SEGMENTS segment;
    short indexOrConstant;
    std::string op;
  };

  Parser(std::string inputFilename);
  ~Parser();
  bool moreInstructions() const;
  const Instruction& getCurrentInstruction() const;
  void reset();
  void advanceInstruction();
  std::string getNextSymbolName();

 private:
  std::ifstream inputFile;
  std::string inputFileBasename;
  bool _moreInstructions;
  Instruction currentInstruction;
  short nextSymbolId;
  void parseInstruction(const std::string& line);
};

#endif