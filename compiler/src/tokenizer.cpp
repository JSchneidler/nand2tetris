#include "tokenizer.hpp"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const char symbolValues[]{'{', '}', '(', ')', '[', ']', '.', ',', ';', '+',
                          '-', '*', '/', '&', '|', '<', '>', '=', '~'};

const std::string keywordValues[]{
    "class", "constructor", "function", "method", "field", "static", "var",
    "int", "char", "boolean", "void", "true", "false", "null",
    "this", "let", "do", "if", "else", "while", "return"};

namespace Tokenizer
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
  std::string Token::getTokenValue() const { return tokenValue; }
  int Token::getTokenValueAsInt() const { return stoi(tokenValue); } // TODO: Fail if token type is not integer const

  std::ostream &operator<<(std::ostream &os, const Token &token)
  {
    std::string tokenType;
    switch (token.getTokenType())
    {
    case TokenType::KEYWORD:
      tokenType = "keyword";
      break;
    case TokenType::SYMBOL:
      tokenType = "symbol";
      break;
    case TokenType::IDENTIFIER:
      tokenType = "identifier";
      break;
    case TokenType::STRING_CONST:
      tokenType = "string";
      break;
    case TokenType::INTEGER_CONST:
      tokenType = "integer";
      break;
    }
    os << tokenType << " " << token.getTokenValue() << std::endl;
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
            // TODO: Parsing error
            std::cout << "Parsing error on line " << lineNumber << " at symbol " << line[0] << ":" << std::endl;
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

        // Parsing error
        else
        {
          std::cout << "Parsing error on line " << lineNumber << " at symbol " << line[0] << ":" << std::endl;
          std::cout << originalLine << std::endl;
          exit(1);
        }
      }

      lineNumber++;
    }

    return tokens;
  }
}