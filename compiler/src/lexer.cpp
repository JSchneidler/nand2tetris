#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer.hpp"

const char symbolValues[]{'{', '}', '(', ')', '[', ']', '<', '>', '.', ',', ';', '+',
                          '-', '*', '/', '&', '|', '=', '~'};

const std::string keywordValues[]{
    "class", "constructor", "function", "method", "field", "static", "var",
    "int", "char", "boolean", "void", "true", "false", "null",
    "this", "let", "do", "if", "else", "while", "return"};

namespace Lexer
{
  Token::Token(const TokenType tokenType, const std::string tokenValue)
  {
    this->tokenType = tokenType;
    this->tokenValue = tokenValue;
  }
  Token::Token(const TokenType tokenType, const char tokenValue)
  {
    this->tokenType = tokenType;
    this->tokenValue = tokenValue;
  }
  TokenType Token::getTokenType() const { return tokenType; }
  std::string Token::getTokenTypeString() const
  {
    switch (tokenType)
    {
    case TokenType::KEYWORD:
      return "keyword";
    case TokenType::SYMBOL:
      return "symbol";
    case TokenType::IDENTIFIER:
      return "identifier";
    case TokenType::STRING_CONST:
      return "stringConstant";
    case TokenType::INTEGER_CONST:
      return "integerConstant";
    }
  }
  std::string Token::getTokenValue() const { return tokenValue; }
  int Token::getTokenValueAsInt() const { return stoi(tokenValue); } // TODO: Fail if token type is not integer const
  std::string Token::getTokenXML() const
  {
    std::string xml = "<" + getTokenTypeString() + "> ";

    if (tokenValue == "<")
      xml += "&lt;";
    else if (tokenValue == ">")
      xml += "&gt;";
    else
      xml += getTokenValue();

    xml += " </" + getTokenTypeString() + ">";
    return xml;
  }

  std::ostream &operator<<(std::ostream &os, const Token &token)
  {
    os << token.getTokenTypeString() << " " << token.getTokenValue() << std::endl;
    return os;
  }
  bool operator==(const Token &token1, const Token &token2)
  {
    if (token1.getTokenType() != token2.getTokenType())
      return false;
    if (token1.getTokenValue() != token2.getTokenValue())
      return false;
    return true;
  }

  bool operator==(const TokenType &tokenType, const Token &token) { return token == tokenType; }
  bool operator==(const Token &token, const TokenType &tokenType) { return token.getTokenType() == tokenType; }

  bool operator==(const Keyword &keyword, const Token &token) { return token == keyword; }
  bool operator==(const Token &token, const Keyword &keyword)
  {
    if (token.getTokenType() != TokenType::KEYWORD) return false;

    switch (keyword)
    {
    case Keyword::BOOLEAN:
      return token.getTokenValue() == "boolean";
    case Keyword::CHAR:
      return token.getTokenValue() == "char";
    case Keyword::CLASS:
      return token.getTokenValue() == "class";
    case Keyword::CONSTRUCTOR:
      return token.getTokenValue() == "constructor";
    case Keyword::DO:
      return token.getTokenValue() == "do";
    case Keyword::ELSE:
      return token.getTokenValue() == "else";
    case Keyword::_FALSE:
      return token.getTokenValue() == "false";
    case Keyword::FIELD:
      return token.getTokenValue() == "field";
    case Keyword::FUNCTION:
      return token.getTokenValue() == "function";
    case Keyword::IF:
      return token.getTokenValue() == "if";
    case Keyword::INT:
      return token.getTokenValue() == "int";
    case Keyword::LET:
      return token.getTokenValue() == "let";
    case Keyword::METHOD:
      return token.getTokenValue() == "method";
    case Keyword::_NULL:
      return token.getTokenValue() == "null";
    case Keyword::RETURN:
      return token.getTokenValue() == "return";
    case Keyword::STATIC:
      return token.getTokenValue() == "static";
    case Keyword::THIS:
      return token.getTokenValue() == "this";
    case Keyword::_TRUE:
      return token.getTokenValue() == "true";
    case Keyword::VAR:
      return token.getTokenValue() == "var";
    case Keyword::VOID:
      return token.getTokenValue() == "void";
    case Keyword::WHILE:
      return token.getTokenValue() == "while";
    default:
      return false;
    }
  }

  bool operator==(const Symbol &symbol, const Token &token) { return token == symbol; }
  bool operator==(const Token &token, const Symbol &symbol)
  {
    if (token.getTokenType() != TokenType::SYMBOL) return false;

    switch (symbol)
    {
    case Symbol::AND:
      return token.getTokenValue() == "&";
    case Symbol::COMMA:
      return token.getTokenValue() == ",";
    case Symbol::DIVIDE:
      return token.getTokenValue() == "/";
    case Symbol::EQUALS:
      return token.getTokenValue() == "=";
    case Symbol::MINUS:
      return token.getTokenValue() == "-";
    case Symbol::MUTIPLY:
      return token.getTokenValue() == "*";
    case Symbol::OR:
      return token.getTokenValue() == "|";
    case Symbol::PERIOD:
      return token.getTokenValue() == ".";
    case Symbol::PLUS:
      return token.getTokenValue() == "+";
    case Symbol::SEMICOLON:
      return token.getTokenValue() == ";";
    case Symbol::TILDE:
      return token.getTokenValue() == "~";
    case Symbol::OPEN_BRACKET:
      return token.getTokenValue() == "[";
    case Symbol::CLOSE_BRACKET:
      return token.getTokenValue() == "]";
    case Symbol::OPEN_CURLY_BRACE:
      return token.getTokenValue() == "{";
    case Symbol::CLOSE_CURLY_BRACE:
      return token.getTokenValue() == "}";
    case Symbol::OPEN_PARENTHESIS:
      return token.getTokenValue() == "(";
    case Symbol::CLOSE_PARENTHESIS:
      return token.getTokenValue() == ")";
    default:
      return false;
    }
  }

  Tokens tokenizeJackFile(const fs::path inputPath)
  {
    Tokens tokens;
    std::string line;
    std::ifstream inputFile{inputPath};

    auto addToken = [&](Token token) {
      tokens.push_back(token);
      std::cout << "Token added: " << token << std::endl;
    };

    bool commentActive = false;

    unsigned int lineNumber = 1;
    while (std::getline(inputFile, line))
    {
      std::string originalLine = line;

      while (line.length() > 0)
      {
        // Is a multi-line comment active?
        if (commentActive)
        {
          size_t commentEnd = line.find("*/");
          // If no comment end, go to next line
          if (commentEnd == std::string::npos)
            break;

          // Otherwise strip the comment from the line
          line = line.substr(commentEnd + 2, std::string::npos);
          commentActive = false;
          continue;
        }

        // Strip leading whitespace
        boost::algorithm::trim_left(line);
        if (line.length() == 0)
          break;

        // Check if comment start
        std::string first2 = line.substr(0, 2);
        if (first2 == "//")
          break;
        else if (first2 == "/*" || line.substr(0, 3) == "/**")
        {
          // Check if a comment end also exists on the line
          size_t commentEnd = line.find("*/");

          // If no comment end, go to next line
          if (commentEnd == std::string::npos)
          {
            commentActive = true;
            break;
          }

          // Otherwise split the remainder of the comment from the line
          line = line.substr(commentEnd + 2, std::string::npos);
        }

        // Check if keyword or identifier
        else if (isalpha(line[0]) || line[0] == '_')
        {
          TokenType tokenType;
          std::string::iterator wordEnd = std::find_if_not(std::begin(line), std::end(line), [](char c) { return isalnum(c) || c == '_'; });
          std::string tokenValue = std::string(line.begin(), wordEnd);

          if (std::find(std::begin(keywordValues), std::end(keywordValues), tokenValue) != std::end(keywordValues))
            tokenType = TokenType::KEYWORD;
          else
            tokenType = TokenType::IDENTIFIER;

          addToken(Token(tokenType, tokenValue));
          line = std::string(wordEnd, line.end());
        }

        // Check if integer constant
        else if (isdigit(line[0]))
        {
          std::string::iterator integerEnd = std::find_if_not(std::begin(line), std::end(line), [](char c) { return isdigit(c); });
          addToken(Token(TokenType::INTEGER_CONST, std::string(line.begin(), integerEnd)));
          line = std::string(integerEnd, line.end());
        }

        // Check if string constant
        else if (line[0] == '"')
        {
          size_t stringEnd = line.find_first_of('"', 1);
          if (stringEnd == std::string::npos)
          {
            // TODO: Lexer error
            std::cout << "LEXING ERROR: Line " << lineNumber << " at symbol " << line[0] << ":" << std::endl;
            std::cout << originalLine << std::endl;
            exit(1);
          }
          else
          {
            addToken(Token(TokenType::STRING_CONST, line.substr(1, stringEnd - 1)));
            line = line.substr(stringEnd + 1, std::string::npos);
          }
        }

        // Check if symbol
        else if (std::find(std::begin(symbolValues), std::end(symbolValues),
                           line[0]) != std::end(symbolValues))
        {
          addToken(Token(TokenType::SYMBOL, line[0]));
          line = line.substr(1, std::string::npos);
        }

        // Lexer error
        else
        {
          std::cout << "LEXING ERROR: Line " << lineNumber << " at symbol " << line[0] << ":" << std::endl;
          std::cout << originalLine << std::endl;
          exit(1);
        }
      }

      lineNumber++;
    }

    return tokens;
  }

  std::string getXML(const Tokens tokens)
  {
    std::string xml = "<tokens>\n";
    for (Token token : tokens)
    {
      xml += token.getTokenXML();
      xml += "\n";
    }
    xml += "</tokens>\n";

    return xml;
  }
}