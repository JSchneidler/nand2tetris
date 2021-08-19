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
    "int",   "char",        "boolean",  "void",   "true",  "false",  "null",
    "this",  "let",         "do",       "if",     "else",  "while",  "return"};

Token::Token(const TokenType tokenType, const std::string tokenValue) {
  this->tokenType = tokenType;
  this->tokenValue = tokenValue;
}

TokenType Token::getTokenType() const { return tokenType; }
std::string Token::getTokenValue() const { return tokenValue; }
int Token::getTokenValueAsInt() const { return stoi(tokenValue); }

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << token.getTokenValue() << std::endl;
  return os;
}

std::string tokenTypeToString(const TokenType type) {
  if (type == TokenType::SYMBOL) return "symbol";
  if (type == TokenType::KEYWORD) return "keyword";
  if (type == TokenType::IDENTIFIER) return "identifier";
  if (type == TokenType::INTEGER_CONST) return "integer constant";
  if (type == TokenType::STRING_CONST) return "string constant";
}

void printTokens(const Tokens tokens) {
  std::cout << "\nTokens:" << std::endl;
  for (Token token : tokens) {
    printf("Type: %s\nValue: %s\n",
           tokenTypeToString(token.getTokenType()).c_str(),
           token.getTokenValue().c_str());
  }
}

void parsingError(const unsigned int lineNumber, const std::string reason) {
  printf("Parsing error occurred on line %u: %s\n", lineNumber, reason.c_str());
  exit(1);
}

Tokens tokenizeJackFile(const fs::path inputPath) {
  Tokens tokens;
  std::string line;
  std::ifstream inputFile{inputPath};

  bool commentActive = false;

  unsigned int lineNumber = 1;
  while (std::getline(inputFile, line)) {
    while (line.length() > 0) {
      bool tokenFound = false;
      bool commentFound = false;

      printf("\nLINE: %s\n", line.c_str());

      // Is a multi-line comment active?
      if (commentActive) {
        // Check if a comment end exists on the line
        size_t commentEnd = line.find("*/");

        // If no comment end, go to next line
        if (commentEnd == std::string::npos) {
          commentActive = true;
          break;
        }

        // Otherwise split the remainder of the comment from the line
        line = line.substr(commentEnd + 2, std::string::npos);
        commentFound = true;
      }

      // Trim leading whitespace
      boost::trim_left(line);

      // If no characters left, go to next line
      if (line.length() == 0) break;

      // Check if comment start
      std::string first2 = line.substr(0, 2);
      if (first2 == "//") {
        break;
      } else if (first2 == "/*" || line.substr(0, 3) == "/**") {
        // Check if a comment end also exists on the line
        size_t commentEnd = line.find("*/");

        // If no comment end, go to next line
        if (commentEnd == std::string::npos) {
          commentActive = true;
          break;
        }

        // Otherwise split the remainder of the comment from the line
        line = line.substr(commentEnd + 2, std::string::npos);
        commentFound = true;
      }

      // Check if symbol
      if (std::find(std::begin(symbolValues), std::end(symbolValues),
                    line[0]) != std::end(symbolValues)) {
        tokenFound = true;

        // Handle string constant
        if (line[0] == '"') {
          size_t end = line.find_first_of('"', 1);

          if (end == std::string::npos)
            parsingError(lineNumber,
                         "Cannot find closing symbol for string constant");

          Token token{TokenType::STRING_CONST, line.substr(1, end - 1)};
          tokens.push_back(token);

          // Remove claimed characters from line
          line = line.substr(end, std::string::npos);
        } else {
          Token token{TokenType::SYMBOL, std::string{line[0]}};
          tokens.push_back(token);

          // Remove claimed characters from line
          line = line.substr(1, std::string::npos);
        }
      }

      // Check if keyword
      for (std::string keyword : keywordValues) {
        if (line.substr(0, keyword.length()) == keyword) {
          tokenFound = true;

          Token token{TokenType::KEYWORD, line.substr(0, keyword.length())};
          tokens.push_back(token);

          // Remove claimed characters from line
          line = line.substr(keyword.length(), std::string::npos);
          break;
        }
      }

      // Check if integer constant
      if (line[0] == '-' || isdigit(line[0])) {
        tokenFound = true;

        // Find end of constant
        size_t end = line.find_first_not_of("0123456789", 1);

        // Remove claimed characters from line
        line = line.substr(end, std::string::npos);
      }

      // Check if identifier
      if (std::isalpha(line[0])) {
        tokenFound = true;

        // TODO
        /*
        std::find_if(line.begin(), line.end(), [](int ch) {
          return !std::isalpha(ch) && !std::isdigit(ch);
        });
        */
      }

      if (!tokenFound && !commentFound)
        parsingError(lineNumber, "No valid tokens found");

      if (tokens.size() > 0) printTokens(tokens);
    }

    lineNumber++;
  }

  return tokens;
}