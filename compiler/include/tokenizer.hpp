#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

const enum class TokenType {
  KEYWORD,
  SYMBOL,
  IDENTIFIER,
  INTEGER_CONST,
  STRING_CONST
};

class Token {
 private:
  TokenType tokenType;
  std::string tokenValue;

 public:
  Token(const TokenType tokenType, const std::string tokenValue);
  TokenType getTokenType() const;
  std::string getTokenValue() const;
  int getTokenValueAsInt() const;
  friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

std::vector<Token> tokenizeJackFile(const fs::path inputPath);