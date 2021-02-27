#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "fs_utils.hpp"
#include "tokenizer.hpp"

namespace fs = std::filesystem;

int main(int, const char *argv[])
{
  if (!argv[1])
  {
    std::cout << "No input file given. Terminating." << std::endl;
    exit(1);
  }

  const fs::path inputPath{argv[1]};
  if (!fs::exists(inputPath))
  {
    std::cout << "Input path does not exist. Terminating." << std::endl;
    exit(1);
  }

  FSUtils::JackFiles inputPaths;
  fs::path outputPath;

  if (inputPath.extension() == ".jack")
  {
    // Single file compilation
    inputPaths = {inputPath};
    outputPath = fs::path(inputPath).stem();
    outputPath += ".xml";
  }
  else if (fs::is_directory(inputPath))
  {
    // Directory compilation
    inputPaths = FSUtils::getJackFilesInDirectory(inputPath);

    outputPath = inputPath.filename();
    if (outputPath == "")
      outputPath = fs::path(inputPath).parent_path().filename();
    outputPath += ".xml";

    if (inputPaths.size() == 0)
    {
      std::cout << "Path contains no .jack files. Path must be a .jack file or "
                   "a directory containing at least one .jack file."
                << std::endl;
      exit(1);
    }
  }
  else
  {
    std::cout
        << "Invalid path passed to compiler. Path must be a .jack file or "
           "a directory containing at least one .jack file."
        << std::endl;
    exit(1);
  }

  for (fs::path path : inputPaths)
  {
    std::cout << "Tokenizing " << path << std::endl;
    Tokenizer::tokenizeJackFile(path);
  }

  return 0;
}