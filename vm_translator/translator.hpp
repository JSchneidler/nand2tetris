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
  std::string initializeStackPointer(const short stackAddress);
  std::string generatePushInstruction(const std::string &segmentStackPointer,
                                      const short index);
  std::string generatePushConstantInstruction(const short value);
  std::string generatePushZeroToStackInstruction();
  std::string generatePushTempInstruction(const short index);
  std::string generatePushPointerInstruction(const short index);
  std::string generatePushStaticInstruction(const short index);
  std::string generatePopInstruction(const std::string &segmentStackPointer,
                                     const short index);
  std::string generatePopTempInstruction(const short index);
  std::string generatePopPointerInstruction(const short index);
  std::string generatePopStaticInstruction(const short index);
  std::string generateArithmeticInstruction(const std::string &op);
  std::string generateLabelInstruction(const std::string &symbol);
  std::string generateConditionalGotoInstruction(const std::string &symbol);
  std::string generateGotoInstruction(const std::string &symbol);
  std::string generateFnDeclInstruction(const std::string &symbol,
                                        const short localVars);
  std::string generateCallInstruction(const std::string &symbol,
                                      const short pushedVars = 0);
  std::string generateReturnInstruction();
  short getCurrentInstructionNumber();

 private:
  short symbolCounter;
  short instructionCount;
  std::string symbolPrefix;
  std::string makeLine(const std::string &string,
                       bool dontIncreaseInstructionNumber = false);
  std::string getNextSymbolName();
  std::string generateSymbol(const short index);
  std::string selectStackPointer();
  std::string incrementStackPointer();
  std::string decrementStackPointer();
  std::string selectStack();
  std::string addLabel(const std::string &string);
  std::string equalityCheck(const Translator::EQUALITY_CHECK_TYPE checkType,
                            const std::string &symbolName);
  std::string selectRegister(const std::string &string);
  std::string selectRegister(const short r);
  std::string pushRegisterToStack(const std::string &string);
};

#endif