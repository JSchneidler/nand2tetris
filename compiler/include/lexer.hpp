#pragma once

#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

#include "node.hpp"

namespace fs = std::filesystem;

namespace Lexer
{
  Node::TerminalNodes tokenizeJackFile(const fs::path inputPath);
  std::string getXML(const Node::TerminalNodes nodes);
}