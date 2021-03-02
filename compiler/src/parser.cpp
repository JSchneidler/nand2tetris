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
  Parser::Parser(Node::TerminalNodes nodes)
  {
    this->nodes = nodes;
  }

  template <typename T>
  void Parser::consumeNode(Node::NonTerminalNode node, const T nodeType, const std::string error)
  {
    if (nodes.front() != nodeType)
      parsingError(error);
    node.addNode(nodes.front());
    nodes.erase(nodes.begin());
  }

  void Parser::consumeNode(Node::NonTerminalNode node, const std::vector<std::variant<Node::Keyword, Node::Symbol, Node::TerminalNode>> allowedTypes, const std::string error)
  {
    for (std::variant<Node::Keyword, Node::Symbol, Node::TerminalNode> type: allowedTypes)
      //if (nodes.front() != type)
        parsingError(error);
    node.addNode(nodes.front());
    nodes.erase(nodes.begin());
  }

  Node::NonTerminalNode parseExpressionList()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::EXPRESSION_LIST);
    return node;
  }

  Node::NonTerminalNode parseTerm()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::TERM);
    return node;
  }

  Node::NonTerminalNode parseExpression()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::EXPRESSION);
    return node;
  }

  Node::NonTerminalNode parseIf()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::IF_STATEMENT);
    return node;
  }

  Node::NonTerminalNode parseReturn()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::RETURN_STATEMENT);
    return node;
  }

  Node::NonTerminalNode parseWhile()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::WHILE_STATEMENT);
    return node;
  }

  Node::NonTerminalNode parseLet()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::LET_STATEMENT);
    return node;
  }

  Node::NonTerminalNode parseDo()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::DO_STATEMENT);
    return node;
  }

  Node::NonTerminalNode parseStatements()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::STATEMENTS);
    return node;
  }

  Node::NonTerminalNode parseVarDec()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::VAR_DEC);
    return node;
  }

  Node::NonTerminalNode parseParameterList()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::PARAMETER_LIST);
    return node;
  }

  Node::NonTerminalNode Parser::parseSubroutine()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::SUBROUTINE_DEC);
    return node;
  }

  Node::NonTerminalNode Parser::parseClassVarDec()
  {
    Node::NonTerminalNode node(Node::NonTerminalNodeType::CLASS_VAR_DEC);

    // Consume static|field
    // consumeNode(nodes, std::vector, "A valid type must follow static|field in a class var dec.");
    // Consume type
    consumeNode(node, Node::Keyword::CLASS, "A valid type must follow static|field in a class var dec.");
    // Consume var name
    consumeNode(node, Node::TerminalNodeType::IDENTIFIER, "An identifier must follow a class var dec type.");

    return node;
  }

  void Parser::parseClass()
  {
    // Class keyword
    consumeNode(rootNode, Node::Keyword::CLASS, "A class must be the first declaration in a Jack file.");
    // Identifier
    consumeNode(rootNode, Node::TerminalNodeType::IDENTIFIER, "An identifier must follow a class declaration.");
    // Open curly brace
    consumeNode(rootNode, Node::Symbol::OPEN_CURLY_BRACE, "An open curly brace must follow a class identifier dec.");

    while (nodes.front() != Node::Symbol::CLOSE_CURLY_BRACE)
    {
      if (nodes.front() == Node::Keyword::STATIC || nodes.front() == Node::Keyword::FIELD)
        rootNode.addNode(parseClassVarDec());
      else if (nodes.front() == Node::Keyword::CONSTRUCTOR || nodes.front() == Node::Keyword::FUNCTION || nodes.front() == Node::Keyword::METHOD)
        rootNode.addNode(parseSubroutine());
      else
        parsingError("A class dec can only contain class var decs or subroutines.");
    }

    // Close curly brace
    consumeNode(rootNode, Node::Symbol::CLOSE_CURLY_BRACE, "A close curly brace must follow the last class var dec or subroutine.");
  }
}