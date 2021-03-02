#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer.hpp"

namespace Lexer
{
  Node::TerminalNodes tokenizeJackFile(const fs::path inputPath)
  {
    Node::TerminalNodes nodes;
    std::string line;
    std::ifstream inputFile{inputPath};

    auto addNode = [&](Node::TerminalNode node) {
      nodes.push_back(node);
      std::cout << "Node added: " << node << std::endl;
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
          Node::TerminalNodeType nodeType;
          std::string::iterator wordEnd = std::find_if_not(std::begin(line), std::end(line), [](char c) { return isalnum(c) || c == '_'; });
          std::string nodeValue = std::string(line.begin(), wordEnd);

          if (std::find(std::begin(keywordValues), std::end(keywordValues), nodeValue) != std::end(keywordValues))
            nodeType = Node::TerminalNodeType::KEYWORD;
          else
            nodeType = Node::TerminalNodeType::IDENTIFIER;

          addNode(Node::TerminalNode(nodeType, nodeValue));
          line = std::string(wordEnd, line.end());
        }

        // Check if integer constant
        else if (isdigit(line[0]))
        {
          std::string::iterator integerEnd = std::find_if_not(std::begin(line), std::end(line), [](char c) { return isdigit(c); });
          addNode(Node::TerminalNode(Node::TerminalNodeType::INTEGER_CONST, std::string(line.begin(), integerEnd)));
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
            addNode(Node::TerminalNode(Node::TerminalNodeType::STRING_CONST, line.substr(1, stringEnd - 1)));
            line = line.substr(stringEnd + 1, std::string::npos);
          }
        }

        // Check if symbol
        else if (std::find(std::begin(symbolValues), std::end(symbolValues),
                           line[0]) != std::end(symbolValues))
        {
          addNode(Node::TerminalNode(Node::TerminalNodeType::SYMBOL, line[0]));
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

    return nodes;
  }

  std::string getXML(const Node::TerminalNodes nodes)
  {
    std::string xml = "<tokens>\n";
    for (Node::TerminalNode node : nodes)
    {
      xml += node.getNodeXML();
      xml += "\n";
    }
    xml += "</tokens>\n";

    return xml;
  }
}