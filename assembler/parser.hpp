#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>
#include <unordered_map>

class Parser {
 public:
  enum instructionType { A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION, NONE };

  Parser(const std::string& inputFilename);
  ~Parser();
  int getCurrentInstructionNumber() const;
  std::string getCurrentInstructionSymbol() const;
  std::string getCurrentInstructionCompField() const;
  std::string getCurrentInstructionDestField() const;
  std::string getCurrentInstructionJmpField() const;
  void reset();
  bool advanceToNextInstruction();
  bool hasMoreInstructions() const;
  bool currentInstructionIsType(instructionType type) const;

 private:
  std::ifstream inputFile;
  int currentInstructionNumber;
  std::string currentInstruction;
  Parser::instructionType currentInstructionType;
  std::string currentInstructionSymbol;
  std::string currentInstructionCompField;
  std::string currentInstructionDestField;
  std::string currentInstructionJmpField;

  void parseCurrentInstruction();
};

const std::unordered_map<std::string, std::string> compMap = {
    {"0", "0101010"},   {"1", "0111111"},   {"-1", "0111010"},
    {"D", "0001100"},   {"A", "0110000"},   {"M", "1110000"},
    {"!D", "0001101"},  {"!A", "0110001"},  {"!M", "1110001"},
    {"-D", "0001111"},  {"-A", "0110011"},  {"-M", "1110011"},
    {"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"},
    {"D-1", "0001110"}, {"A-1", "0110010"}, {"M-1", "1110010"},
    {"D+A", "0000010"}, {"D+M", "1000010"}, {"D-A", "0010011"},
    {"D-M", "1010011"}, {"A-D", "0000111"}, {"M-D", "1000111"},
    {"D&A", "0000000"}, {"D&M", "1000000"}, {"D|A", "0010101"},
    {"D|M", "1010101"},
};

const std::unordered_map<std::string, std::string> destMap = {
    {"M", "001"},  {"D", "010"},  {"MD", "011"}, {"A", "100"},
    {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}};

const std::unordered_map<std::string, std::string> jmpMap = {
    {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"},
    {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};

std::string trim(const std::string& string);
std::string stripComment(const std::string& string);

#endif