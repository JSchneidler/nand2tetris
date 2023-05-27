#include "parser.hpp"

/*
These are the unit tests for the parser module.
The translator module is not unit tested as there are a
multitude of valid solutions for each method and I can't think of an elegant way
to unit test them. These are better left for manual and integration testing.

Each fuction performs unit tests on a specific module and returns 0 if they
pass, and 1 otherwise.
*/

int fail(const std::string &reason);

int parserTest()
{
  Parser parser{"test.vm"};

  // getRawInstruction()
  if (parser.getRawInstruction() != "function SimpleFunction.test 2")
    return fail("First line raw instruction is not expected value");

  // reset()
  parser.advanceInstruction();
  parser.reset();
  if (parser.getRawInstruction() != "function SimpleFunction.test 2")
    return fail(
        "After parser.reset(), instruction is not equal to first instruction");

  // moreInstructions()
  if (!parser.moreInstructions())
    return fail("Parser should have more instructions");

  // advanceInstruction()
  for (parser.reset(); parser.moreInstructions(); parser.advanceInstruction())
  {
  }
  if (parser.getRawInstruction() != "return")
    return fail(
        "After advanceInstruction iteration done, instruction should be "
        "'return'");

  // getCurrentInstruction()
  parser.reset();

  Parser::Instruction instruction{parser.getCurrentInstruction()};
  if (instruction.type != Parser::INSTRUCTION_TYPES::FN_DECL_INSTRUCTION ||
      instruction.indexOrConstant != 2 ||
      instruction.symbol != "SimpleFunction.test")
    return fail(
        "Instruction returned from parser does not match expected values");

  return 0;
}

int main()
{
  if (parserTest())
    return 1;

  printf("Success");
  return 0;
}

int fail(const std::string &reason)
{
  printf("%s\n", reason.c_str());
  return 1;
}