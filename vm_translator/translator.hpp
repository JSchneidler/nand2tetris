#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <string>
#include "parser.hpp"

class Translator {
 public:
  enum EQUALITY_CHECK_TYPE { EQ_CHECK, GT_CHECK, LT_CHECK };

  Translator();
  void restartSymbolCounter();
  void setSymbolPrefix(const std::string &prefix);
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
  static std::string generateLabelInstruction(const std::string &symbol);
  static std::string generateConditionalGotoInstruction(
      const std::string &symbol);
  static std::string generateGotoInstruction(const std::string &symbol);
  static std::string generateFnDeclInstruction(const std::string &symbol,
                                               const short localVars);
  static std::string generateCallInstruction(const std::string &symbol,
                                             const short pushedVars);
  static std::string generateReturnInstruction(const std::string &symbol);

 private:
  short symbolCounter;
  std::string symbolPrefix;
  std::string getNextSymbolName();
  std::string generateSymbol(const short index);
};

#endif