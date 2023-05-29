#include "parser.hpp"
#include <iostream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>

Parser::Parser(const std::string &inputFilename)
    : inputFile(std::ifstream(inputFilename)),
      instructionNumber(0),
      _moreCommands{true},
      command(""),
      commandSymbol(""),
      instructionCompField(""),
      instructionDestField(""),
      instructionJmpField(""),
      commandType(NONE)
{
  if (!inputFile.good() || inputFile.bad() || inputFile.fail() ||
      !inputFile.is_open())
    throw std::runtime_error("Invalid input file");
  advanceCommand(true);
}

Parser::~Parser() { inputFile.close(); }

int Parser::getInstructionNumber() const { return instructionNumber; }

std::string Parser::getCommand() const { return command; }

std::string Parser::getCommandSymbol() const { return commandSymbol; }

std::string Parser::getInstructionCompField() const
{
  return instructionCompField;
}

std::string Parser::getInstructionDestField() const
{
  return instructionDestField;
}

std::string Parser::getInstructionJmpField() const
{
  return instructionJmpField;
}

// Sets input file iterator back to beginning
void Parser::reset()
{
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);
  instructionNumber = 0;
  _moreCommands = true;

  advanceCommand(true);
}

// Advances parser state to the next command in the file.
void Parser::advanceCommand(bool init /*= false*/)
{
  // Read lines until we find a valid command
  std::string line;
  while (std::getline(inputFile, line))
  {
    line = stripComment(line);
    boost::trim(line);

    if (line != "")
    {
      // Parse out command fields and any symbols
      parseCommand(line);

      // Only increase command number if it is an A or C command, not an
      // internal L command.
      if (!commandIsType(L_COMMAND))
        instructionNumber = (init ? 0 : instructionNumber + 1);

      // Exit
      return;
    }
  }
  _moreCommands = false;
}

bool Parser::moreCommands() const { return _moreCommands; }

bool Parser::commandIsType(commandTypes type) const
{
  return commandType == type;
}

// Parses a command into fields
void Parser::parseCommand(const std::string &line)
{
  // Set command to raw command
  command = line;

  if (line[0] == '(')
  {
    commandType = L_COMMAND;
    commandSymbol = line.substr(1, line.size() - 2);
  }
  else if (line[0] == '@')
  {
    commandType = A_INSTRUCTION;
    commandSymbol = line.substr(1);
  }
  else
  {
    commandType = C_INSTRUCTION;
    commandSymbol = "";

    size_t destFieldSplitter{line.find_first_of('=')};
    size_t jmpFieldSplitter{line.find_first_of(';')};
    size_t compStart{0};
    size_t compEnd{line.length()};

    // If dest field, parse
    if (destFieldSplitter != std::string::npos)
    {
      instructionDestField = destMap.at(line.substr(0, destFieldSplitter));
      compStart = destFieldSplitter + 1; // Set start of comp field
    }
    else
      instructionDestField = "000";

    // If jmp field, parse
    if (jmpFieldSplitter != std::string::npos)
    {
      instructionJmpField = jmpMap.at(line.substr(jmpFieldSplitter + 1));
      compEnd = jmpFieldSplitter; // Set end of comp field
    }
    else
      instructionJmpField = "000";

    // Parse comp field
    instructionCompField = compMap.at(line.substr(compStart, compEnd));
  }
}

// Strips a comment off the end of a line
std::string stripComment(const std::string &string)
{
  if (string == "")
    return string;

  size_t i{string.find("//")};
  if (i != std::string::npos)
  {
    return string.substr(0, i);
  }

  return string;
}