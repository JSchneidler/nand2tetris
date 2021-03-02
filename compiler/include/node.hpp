#pragma once

#include <string>
#include <vector>
#include <variant>

const char symbolValues[]{'{', '}', '(', ')', '[', ']', '<', '>', '.', ',', ';', '+',
                          '-', '*', '/', '&', '|', '=', '~'};

const std::string keywordValues[]{
    "class", "constructor", "function", "method", "field", "static", "var",
    "int", "char", "boolean", "void", "true", "false", "null",
    "this", "let", "do", "if", "else", "while", "return"};

namespace Node
{
  enum class TerminalNodeType
  {
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INTEGER_CONST,
    STRING_CONST
  };

  enum class Keyword
  {
    BOOLEAN,
    CHAR,
    CLASS,
    CONSTRUCTOR,
    DO,
    ELSE,
    _FALSE,
    FIELD,
    FUNCTION,
    IF,
    INT,
    LET,
    METHOD,
    _NULL,
    RETURN,
    STATIC,
    THIS,
    _TRUE,
    VAR,
    VOID,
    WHILE
  };

  enum class Symbol
  {
    AND,
    COMMA,
    DIVIDE,
    EQUALS,
    MINUS,
    MUTIPLY,
    OR,
    PERIOD,
    PLUS,
    SEMICOLON,
    TILDE,
    OPEN_CURLY_BRACE,
    CLOSE_CURLY_BRACE,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_BRACKET,
    CLOSE_BRACKET,
  };

  enum class NonTerminalNodeType
  {
    CLASS,
    CLASS_VAR_DEC,
    SUBROUTINE_DEC,
    PARAMETER_LIST,
    SUBROUTINE_BODY,
    VAR_DEC,
    STATEMENTS,
    WHILE_STATEMENT,
    IF_STATEMENT,
    RETURN_STATEMENT,
    LET_STATEMENT,
    DO_STATEMENT,
    EXPRESSION,
    TERM,
    EXPRESSION_LIST
  };

  enum class NodeType
  {
    TERMINAL,
    NON_TERMINAL
  };

  class Node
  {
    NodeType nodeType;

    public:
      Node(NodeType nodeType);
      NodeType getNodeType();
  };

  typedef std::vector<Node> Nodes;

  class TerminalNode : public Node
  {
  private:
    TerminalNodeType nodeType;
    std::string nodeValue;

  public:
    TerminalNode(const TerminalNodeType nodeType, const std::string nodeValue);
    TerminalNode(const TerminalNodeType nodeType, const char nodeValue);
    TerminalNodeType getNodeType() const;
    std::string getNodeTypeString() const;
    std::string getNodeValue() const;
    int getNodeValueAsInt() const;
    std::string getNodeXML() const;

    friend std::ostream &operator<<(std::ostream &os, const TerminalNode &node);
    friend bool operator==(const TerminalNode &node1, const TerminalNode &node2);

    friend bool operator==(const TerminalNodeType &nodeType, const TerminalNode &node);
    friend bool operator==(const TerminalNode &node, const TerminalNodeType &nodeType);

    friend bool operator==(const Keyword &keyword, const TerminalNode &node);
    friend bool operator==(const TerminalNode &node, const Keyword &keyword);

    friend bool operator==(const Symbol &symbol, const TerminalNode &node);
    friend bool operator==(const TerminalNode &node, const Symbol &symbol);
  };

  typedef std::vector<TerminalNode> TerminalNodes;

  class NonTerminalNode : public Node
  {
  private:
    Nodes nodes;
    NonTerminalNodeType nodeType;

  public:
    NonTerminalNode(const NonTerminalNodeType nodeType);
    Nodes getNodes() const;
    NonTerminalNodeType getNodeType() const;
    void addNode(const Node node);
  };
}