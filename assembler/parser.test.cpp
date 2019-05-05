#include "parser.hpp"
#include <iostream>
#include <string>

// std::string trim(const std::string & string);
// std::string stripComment(const std::string & string);

std::string emptyString = "";
std::string leadingAndTrailingWhitespace = "     hello     ";

std::string comment = "// This is a comment";
std::string endOfLineComment = "int i = 10;            // This is a comment";

int main()
{
  // Shouldn't crash
  trim(emptyString);
  
  std::cout << "Before: {" << leadingAndTrailingWhitespace << "}" << std::endl;
  std::cout << "After: {" << trim(leadingAndTrailingWhitespace) << "}" << std::endl;

  std::cout << "Before: {" << comment << "}" << std::endl;
  std::cout << "After: {" << stripComment(comment) << "}" << std::endl;

  std::cout << "Before: {" << endOfLineComment << "}" << std::endl;
  std::cout << "After: {" << stripComment(endOfLineComment) << "}" << std::endl;

  std::cout << "Before: {" << endOfLineComment << "}" << std::endl;
  std::cout << "After: {" << trim(stripComment(endOfLineComment)) << "}" << std::endl;

  return 0;
}