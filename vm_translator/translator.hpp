#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <string>
#include "parser.hpp"

class Translator
{
public:
  enum EQUALITY_CHECK_TYPE
  {
    EQ_CHECK,
    GT_CHECK,
    LT_CHECK
  };

  Translator();
  void setSymbolPrefix(const std::string &prefix);
  std::string initializeStackPointer(const int stackAddress);
  std::string generatePushInstruction(const std::string &segmentStackPointer,
                                      const int index);
  std::string generatePushConstantInstruction(const int value);
  std::string generatePushZeroToStackInstruction();
  std::string generatePushTempInstruction(const int index);
  std::string generatePushPointerInstruction(const int index);
  std::string generatePushStaticInstruction(const int index);
  std::string generatePopInstruction(const std::string &segmentStackPointer,
                                     const int index);
  std::string generatePopTempInstruction(const int index);
  std::string generatePopPointerInstruction(const int index);
  std::string generatePopStaticInstruction(const int index);
  std::string generateArithmeticInstruction(const std::string &op);
  std::string generateLabelInstruction(const std::string &symbol);
  std::string generateConditionalGotoInstruction(const std::string &symbol);
  std::string generateGotoInstruction(const std::string &symbol);
  std::string generateFnDeclInstruction(const std::string &symbol,
                                        const int localVars);
  std::string generateCallInstruction(const std::string &symbol,
                                      const int pushedVars = 0);
  std::string generateReturnInstruction();
  int getCurrentInstructionNumber();

private:
  int equalitySymbolId;
  int callSymbolId;
  int instructionCount;
  std::string symbolPrefix;
  std::string currentFunctionName;
  std::string makeLine(const std::string &string,
                       bool dontIncreaseInstructionNumber = false);
  std::string getNextEqualitySymbolId();
  std::string generateStaticSymbol(const int index);
  std::string selectStackPointer();
  std::string incrementStackPointer();
  std::string decrementStackPointer();
  std::string selectStack();
  std::string addLabel(const std::string &string);
  std::string equalityCheck(const Translator::EQUALITY_CHECK_TYPE checkType,
                            const std::string &symbolName);
  std::string selectRegister(const std::string &string);
  std::string selectRegister(const int r);
  std::string pushRegisterToStack(const std::string &string);
  void setCurrentFunctionName(const std::string &string);
};

#endif