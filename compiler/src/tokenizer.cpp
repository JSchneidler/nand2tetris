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

std::vector<Token> tokenizeJackFile(const fs::path inputPath) {
  std::vector<Token> tokens;
  std::string line;
  std::ifstream inputFile{inputPath};

  bool commentActive = false;

  while (std::getline(inputFile, line)) {
    // Strip leading whitespace
    boost::algorithm::trim_left(line);
    for (int i = 0; i != std::string::npos; i++) {
      // Is a multi-line comment active?
      if (commentActive) {
        size_t commentEnd = line.find("*/");
        // If no comment end, go to next line
        if (commentEnd == std::string::npos) break;
        // Otherwise split the remainder of the comment from the line
        line = line.substr(commentEnd, std::string::npos);
      }

      // Check if comment start
      std::string first2 = line.substr(i, 2);
      if (first2 == "//")
        break;
      else if (first2 == "/*" || line.substr(i, 3) == "/**") {
        commentActive = true;
        // Check if a comment end also exists on the line
        size_t commentEnd = line.find("*/");
        // If no comment end, go to next line
        if (commentEnd == std::string::npos) break;
        // Otherwise split the remainder of the comment from the line
        line = line.substr(commentEnd, std::string::npos);
      }

      // Strip trailing whitespace
      boost::algorithm::trim_right(line);

      // Check if symbol
      if (std::find(std::begin(symbolValues), std::end(symbolValues),
                    line[i]) != std::end(symbolValues)) {
        // TODO: Handle string constant
        Token token{TokenType::SYMBOL, std::string{line[i]}};
        tokens.push_back(token);
        std::cout << token << std::endl;
      }

      // TODO: Check if keyword

      // Check if integer constant
      else if (isdigit(line[i])) {
        // TODO: Handle integer constant
      }

      // TODO: Check if identifier

      else {
        // TODO: Parsing error
        std::cout << "Parsing error on following line: " << line << std::endl;
        exit(1);
      }
    }
  }

  return tokens;
}