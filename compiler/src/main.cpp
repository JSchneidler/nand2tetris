#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "fs_utils.hpp"
#include "lexer.hpp"
#include "parser.hpp"

namespace fs = std::filesystem;

static constexpr char USAGE[] =
R"(Jack Compiler

Usage:
  JackC input_path [options]

input_path: A path to a .jack file or directory containing one or more .jack files

Options:
  No supported options at the moment.
)";

void fatalError(std::string err)
{
  std::cout << USAGE << std::endl;
  std::cout << "PROGRAM ERROR: " << err << std::endl;
  exit(1);
}

int main(int, const char *argv[])
{
  if (!argv[1])
    fatalError("No input file given.");

  const fs::path inputPath{argv[1]};
  if (!fs::exists(inputPath))
    fatalError("Input path does not exist.");

  FSUtils::JackFiles inputPaths;

  // Single file compilation
  if (inputPath.extension() == ".jack")
    inputPaths = {inputPath};
  else if (fs::is_directory(inputPath))
  {
    // Directory compilation
    inputPaths = FSUtils::getJackFilesInDirectory(inputPath);

    if (inputPaths.size() == 0)
      fatalError("Path contains no .jack files. Path must be a .jack file or "
                   "a directory containing at least one .jack file.");
  }
  else
    fatalError("Invalid path passed to compiler. Path must be a .jack file or "
           "a directory containing at least one .jack file.");

  for (fs::path path : inputPaths)
  {
    std::cout << "Compiling " << path << std::endl;
    Node::TerminalNodes nodes = Lexer::tokenizeJackFile(path);
    fs::path outputPath (path.replace_filename(path.stem().string() + ".xml"));
    std::ofstream outputFile(outputPath);
    outputFile << Lexer::getXML(nodes);
    outputFile.close();
  }

  return 0;
}