#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>
#include <unordered_map>

class Parser {
 public:
  enum commandTypes { A_INSTRUCTION, C_INSTRUCTION, L_COMMAND, NONE };

  Parser(const std::string& inputFilename);
  ~Parser();
  int getInstructionNumber() const;
  std::string getCommand() const;
  std::string getCommandSymbol() const;
  std::string getInstructionCompField() const;
  std::string getInstructionDestField() const;
  std::string getInstructionJmpField() const;
  void reset();
  void advanceCommand(bool init = false);
  bool moreCommands() const;
  bool commandIsType(commandTypes type) const;

 private:
  std::ifstream inputFile;
  int instructionNumber;
  bool _moreCommands;
  std::string command;
  std::string commandSymbol;
  std::string instructionCompField;
  std::string instructionDestField;
  std::string instructionJmpField;
  Parser::commandTypes commandType;

  void parseCommand(const std::string& line);
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