#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>

class Parser
{
public:
  enum INSTRUCTION_TYPES
  {
    LABEL_INSTRUCTION,
    PUSH_INSTRUCTION,
    POP_INSTRUCTION,
    ARITHMETIC_INSTRUCTION,
    IF_INSTRUCTION,
    GOTO_INSTRUCTION,
    FN_DECL_INSTRUCTION,
    CALL_INSTRUCTION,
    RETURN_INSTRUCTION,
    NONE_INSTRUCTION
  };

  enum SEGMENTS
  {
    CONSTANT_SEGMENT,
    LOCAL_SEGMENT,
    ARGUMENT_SEGMENT,
    THIS_SEGMENT,
    THAT_SEGMENT,
    POINTER_SEGMENT,
    TEMP_SEGMENT,
    STATIC_SEGMENT,
    NONE_SEGMENT
  };

  struct Instruction
  {
    INSTRUCTION_TYPES type;
    SEGMENTS segment;
    int indexOrConstant;
    std::string op;
    std::string symbol;
  };

  Parser(std::string inputFilename);
  ~Parser();
  bool moreInstructions() const;
  const Instruction &getCurrentInstruction() const;
  void reset();
  void advanceInstruction();
  std::string getRawInstruction();

private:
  std::ifstream inputFile;
  std::string inputFileBasename;
  bool _moreInstructions;
  Instruction currentInstruction;
  std::string rawInstruction;
  void parseInstruction(const std::string &line);
};

bool operator==(const Parser::Instruction &left,
                const Parser::Instruction &right);
bool operator!=(const Parser::Instruction &left,
                const Parser::Instruction &right);

#endif