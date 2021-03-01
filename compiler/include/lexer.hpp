#pragma once

#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace Lexer
{
  enum class TokenType
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

  class Token
  {
  private:
    TokenType tokenType;
    std::string tokenValue;

  public:
    Token(const TokenType tokenType, const std::string tokenValue);
    Token(const TokenType tokenType, const char tokenValue);
    TokenType getTokenType() const;
    std::string getTokenTypeString() const;
    std::string getTokenValue() const;
    int getTokenValueAsInt() const;
    std::string getTokenXML() const;

    friend std::ostream &operator<<(std::ostream &os, const Token &token);
    friend bool operator==(const Token &token1, const Token &token2);

    friend bool operator==(const TokenType &tokenType, const Token &token);
    friend bool operator==(const Token &token, const TokenType &tokenType);

    friend bool operator==(const Keyword &keyword, const Token &token);
    friend bool operator==(const Token &token, const Keyword &keyword);

    friend bool operator==(const Symbol &symbol, const Token &token);
    friend bool operator==(const Token &token, const Symbol &symbol);
  };

  typedef std::vector<Token> Tokens;

  Tokens tokenizeJackFile(const fs::path inputPath);
  std::string getXML(const Tokens tokens);
}