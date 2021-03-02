#include <iostream>

#include "node.hpp"

namespace Node
{
  Node::Node(NodeType nodeType)
  {
    this->nodeType = nodeType;
  }

  NodeType Node::getNodeType()
  {
    return nodeType;
  }

  TerminalNode::TerminalNode(const TerminalNodeType nodeType, const std::string nodeValue) : Node {NodeType::TERMINAL}
  {
    this->nodeType = nodeType;
    this->nodeValue = nodeValue;
  }
  TerminalNode::TerminalNode(const TerminalNodeType nodeType, const char nodeValue) : Node {NodeType::TERMINAL}
  {
    this->nodeType = nodeType;
    this->nodeValue = nodeValue;
  }
  TerminalNodeType TerminalNode::getNodeType() const { return nodeType; }
  std::string TerminalNode::getNodeTypeString() const
  {
    switch (nodeType)
    {
    case TerminalNodeType::KEYWORD:
      return "keyword";
    case TerminalNodeType::SYMBOL:
      return "symbol";
    case TerminalNodeType::IDENTIFIER:
      return "identifier";
    case TerminalNodeType::STRING_CONST:
      return "stringConstant";
    case TerminalNodeType::INTEGER_CONST:
      return "integerConstant";
    }
  }
  std::string TerminalNode::getNodeValue() const { return nodeValue; }
  int TerminalNode::getNodeValueAsInt() const { return stoi(nodeValue); } // TODO: Fail if node type is not integer const
  std::string TerminalNode::getNodeXML() const
  {
    std::string xml = "<" + getNodeTypeString() + "> ";

    if (nodeValue == "<")
      xml += "&lt;";
    else if (nodeValue == ">")
      xml += "&gt;";
    else
      xml += getNodeValue();

    xml += " </" + getNodeTypeString() + ">";
    return xml;
  }

  std::ostream &operator<<(std::ostream &os, const TerminalNode &node)
  {
    os << node.getNodeTypeString() << " " << node.getNodeValue() << std::endl;
    return os;
  }
  bool operator==(const TerminalNode &node1, const TerminalNode &node2)
  {
    if (node1.getNodeType() != node2.getNodeType())
      return false;
    if (node1.getNodeValue() != node2.getNodeValue())
      return false;
    return true;
  }

  bool operator==(const TerminalNodeType &nodeType, const TerminalNode &node) { return node == nodeType; }
  bool operator==(const TerminalNode &node, const TerminalNodeType &nodeType) { return node.getNodeType() == nodeType; }

  bool operator==(const Keyword &keyword, const TerminalNode &node) { return node == keyword; }
  bool operator==(const TerminalNode &node, const Keyword &keyword)
  {
    if (node.getNodeType() != TerminalNodeType::KEYWORD) return false;

    switch (keyword)
    {
    case Keyword::BOOLEAN:
      return node.getNodeValue() == "boolean";
    case Keyword::CHAR:
      return node.getNodeValue() == "char";
    case Keyword::CLASS:
      return node.getNodeValue() == "class";
    case Keyword::CONSTRUCTOR:
      return node.getNodeValue() == "constructor";
    case Keyword::DO:
      return node.getNodeValue() == "do";
    case Keyword::ELSE:
      return node.getNodeValue() == "else";
    case Keyword::_FALSE:
      return node.getNodeValue() == "false";
    case Keyword::FIELD:
      return node.getNodeValue() == "field";
    case Keyword::FUNCTION:
      return node.getNodeValue() == "function";
    case Keyword::IF:
      return node.getNodeValue() == "if";
    case Keyword::INT:
      return node.getNodeValue() == "int";
    case Keyword::LET:
      return node.getNodeValue() == "let";
    case Keyword::METHOD:
      return node.getNodeValue() == "method";
    case Keyword::_NULL:
      return node.getNodeValue() == "null";
    case Keyword::RETURN:
      return node.getNodeValue() == "return";
    case Keyword::STATIC:
      return node.getNodeValue() == "static";
    case Keyword::THIS:
      return node.getNodeValue() == "this";
    case Keyword::_TRUE:
      return node.getNodeValue() == "true";
    case Keyword::VAR:
      return node.getNodeValue() == "var";
    case Keyword::VOID:
      return node.getNodeValue() == "void";
    case Keyword::WHILE:
      return node.getNodeValue() == "while";
    default:
      return false;
    }
  }

  bool operator==(const Symbol &symbol, const TerminalNode &node) { return node == symbol; }
  bool operator==(const TerminalNode &node, const Symbol &symbol)
  {
    if (node.getNodeType() != TerminalNodeType::SYMBOL) return false;

    switch (symbol)
    {
    case Symbol::AND:
      return node.getNodeValue() == "&";
    case Symbol::COMMA:
      return node.getNodeValue() == ",";
    case Symbol::DIVIDE:
      return node.getNodeValue() == "/";
    case Symbol::EQUALS:
      return node.getNodeValue() == "=";
    case Symbol::MINUS:
      return node.getNodeValue() == "-";
    case Symbol::MUTIPLY:
      return node.getNodeValue() == "*";
    case Symbol::OR:
      return node.getNodeValue() == "|";
    case Symbol::PERIOD:
      return node.getNodeValue() == ".";
    case Symbol::PLUS:
      return node.getNodeValue() == "+";
    case Symbol::SEMICOLON:
      return node.getNodeValue() == ";";
    case Symbol::TILDE:
      return node.getNodeValue() == "~";
    case Symbol::OPEN_BRACKET:
      return node.getNodeValue() == "[";
    case Symbol::CLOSE_BRACKET:
      return node.getNodeValue() == "]";
    case Symbol::OPEN_CURLY_BRACE:
      return node.getNodeValue() == "{";
    case Symbol::CLOSE_CURLY_BRACE:
      return node.getNodeValue() == "}";
    case Symbol::OPEN_PARENTHESIS:
      return node.getNodeValue() == "(";
    case Symbol::CLOSE_PARENTHESIS:
      return node.getNodeValue() == ")";
    default:
      return false;
    }
  }

  NonTerminalNode::NonTerminalNode(const NonTerminalNodeType nodeType) : Node {NodeType::NON_TERMINAL}
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

  void NonTerminalNode::addNode(const Node node)
  {
    nodes.push_back(node);
  }
}