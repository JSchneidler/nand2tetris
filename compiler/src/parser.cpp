#include <iostream>
#include <string>

#include "parser.hpp"
#include "lexer.hpp"

void parsingError(std::string error)
{
  std::cout << "PARSING ERROR: " << error << std::endl;
  exit(1);
}

namespace Parser
{
  NonTerminalNode::NonTerminalNode(NonTerminalNodeType nodeType)
  {
    this->nodeType = nodeType;
  }

  Nodes NonTerminalNode::getNodes() const
  {
    return nodes;
  }

  NonTerminalNodeType NonTerminalNode::getNodeType() const
  {
    return nodeType;
  }

  NonTerminalNode parseClass(Lexer::Tokens tokens)
  {
    if (!(tokens.front() == Lexer::Keyword::CLASS))
      parsingError("A class must be the first declaration in a Jack file.");
    tokens.erase(tokens.begin());

    if (!tokens.front().isIdentifier())
      parsingError("An identifier must come immediately after a class declaration.");
    tokens.erase(tokens.begin());
    NonTerminalNode rootNode (NonTerminalNodeType::CLASS);
    return rootNode;
  }

  NonTerminalNode parseTokens(Lexer::Tokens tokens)
  {
    return parseClass(tokens);
  }
}