#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <string>
#include "parser.hpp"

class Translator {
 public:
  enum EQUALITY_CHECK_TYPE { EQ_CHECK, GT_CHECK, LT_CHECK };

  Translator();
  static std::string initializeStackPointer(const short stackAddress);
  static std::string generatePushInstruction(
      const std::string &segmentStackPointer, const short index);
  static std::string generatePushConstantInstruction(const short value);
  static std::string generatePushTempInstruction(const short index);
  static std::string generatePushPointerInstruction(const short index);
  std::string generatePushStaticInstruction(const short index);
  static std::string generatePopInstruction(
      const std::string &segmentStackPointer, const short index);
  static std::string generatePopTempInstruction(const short index);
  static std::string generatePopPointerInstruction(const short index);
  std::string generatePopStaticInstruction(const short index);
  std::string generateArithmeticInstruction(const std::string &op);
  void restartSymbolCounter();
  void setSymbolPrefix(const std::string &prefix);

 private:
  short symbolCounter;
  std::string symbolPrefix;
  std::string getNextSymbolName();
  std::string generateSymbol(const short index);
};

#endif