#include <iostream>
#include "parser.hpp"
#include "symboltable.hpp"

int main() {
  // Test Symbol Table

  // Test trimWhitespace()
  std::cout << "1: " << trimWhitespace("Hello") << "|" << std::endl;
  std::cout << "2: " << trimWhitespace("Hello  ") << "|" << std::endl;
  std::cout << "3: " << trimWhitespace("  Hello") << "|" << std::endl;
  std::cout << "4: " << trimWhitespace("  Hello  ") << "|" << std::endl;
  // Test stripComment()
  std::cout << "5: " << stripComment("Hello") << "|" << std::endl;
  std::cout << "6: " << stripComment("Hello // Comment") << "|" << std::endl;
  std::cout << "7: " << stripComment("Hello// Comment") << "|" << std::endl;
  // Test trimWhitespace(stripComment())
  std::cout << "8: " << trimWhitespace(stripComment("  Hello    // Comment"))
            << "|" << std::endl;

  return 0;
}