#include "parser.hpp"
#include "symboltable.hpp"

/*
These are the unit tests for the symbol table and parser modules.

Each fuction performs unit tests on a specific module and returns 0 if they
pass, and 1 otherwise.
*/

int fail(const std::string &reason);

int symbolTableTest() {
  SymbolTable symbolTable{};

  // contains()
  if (symbolTable.contains("test"))
    return fail("symbolTable should not contain 'test' key");
  if (!symbolTable.contains("SP"))
    return fail("symbolTable should contain 'SP' key");

  // getSymbolValue()
  if (symbolTable.getSymbolValue("R1") != 0x1)
    return fail("symbolTable key 'R1' should have a value of 0x1");

  // addSymbol()
  symbolTable.addSymbol("test", 0xF);
  if (!symbolTable.contains("test"))
    return fail("symbolTable should contain key 'test'");
  if (symbolTable.getSymbolValue("test") != 0xF)
    return fail("symbolTable key 'test' should have a value of 0xF");

  try {
    symbolTable.addSymbol("SP", 0x1);
    return fail(
        "symbolTable should throw an std::invalid_argument when attempting to "
        "add an existing key");
  } catch (std::invalid_argument &) {
  }

  return 0;
}

int parserTest() {
  Parser parser{"test.asm"};

  // First instruction should be "@0", all comments and whitespace should be
  // stripped already
  if (parser.getCommand() != "@0")
    return fail("First instruction should be '@0'");

  // advanceCommand(), next command should be "D=M"
  parser.advanceCommand();
  if (parser.getCommand() != "D=M")
    return fail("Second instruction should be 'D=M'");

  // reset(), should reset parser back to first command
  parser.reset();
  if (parser.getCommand() != "@0")
    return fail("After parser.reset(), instruction should be equal to '@0'");

  // instructionNumber, iterate to third instruction. instructionNumber
  // should be 2
  parser.reset();
  parser.advanceCommand();
  parser.advanceCommand();
  if (parser.getInstructionNumber() != 2)
    return fail(
        "After iterating to third instruction, instructionNumber should be 2");

  // getCommandSymbol()
  parser.reset();
  parser.advanceCommand();
  parser.advanceCommand();
  if (parser.getCommandSymbol() != "INFINITE_LOOP")
    return fail(
        "On third instruction, parser.getCommandSymbol() should return "
        "'INFINITE_LOOP'");

  // getInstructionCompField()
  parser.reset();
  parser.advanceCommand();
  if (parser.getInstructionCompField() != "1110000")
    return fail("Instruction comp field should be '1110000'");

  // getInstructionDestField()
  parser.reset();
  parser.advanceCommand();
  if (parser.getInstructionDestField() != "010")
    return fail("Instruction dest field should be '010'");

  // getInstructionJmpField()
  parser.reset();
  parser.advanceCommand();
  parser.advanceCommand();
  parser.advanceCommand();
  if (parser.getInstructionJmpField() != "110")
    return fail("Instruction jmp field should be '110'");

  // moreCommands()
  parser.reset();
  if (!parser.moreCommands()) return fail("Parser should have more commands");

  for (parser.reset(); parser.moreCommands(); parser.advanceCommand()) {
  }
  if (parser.moreCommands())
    return fail(
        "Parser should not have more commands after being exhausted in a loop");

  // commandIsType()
  // A instruction
  parser.reset();
  if (!parser.commandIsType(Parser::A_INSTRUCTION))
    return fail("First instruction is an A instruction");

  // C instruction
  parser.advanceCommand();
  if (!parser.commandIsType(Parser::C_INSTRUCTION))
    return fail("Second instruction is an C instruction");

  // L command
  parser.reset();
  for (int i = 0; i < 10; i++) parser.advanceCommand();
  if (!parser.commandIsType(Parser::L_COMMAND))
    return fail("10th instruction is an L command");

  return 0;
}

int main() {
  if (symbolTableTest()) return 1;
  if (parserTest()) return 1;

  printf("Success");
  return 0;
}

int fail(const std::string &reason) {
  printf("%s\n", reason.c_str());
  return 1;
}