#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "fs_utils.hpp"
#include "tokenizer.hpp"

namespace fs = std::filesystem;

int main(int, const char *argv[]) {
  if (!argv[1]) {
    std::cout << "No input file given. Terminating." << std::endl;
    exit(1);
  }

  const fs::path inputPath{argv[1]};
  fs::path outputPath;

  if (!fs::exists(inputPath)) {
    std::cout << "Input path does not exist. Terminating." << std::endl;
    exit(1);
  }

  // Create list of files to read
  std::vector<fs::path> inputPaths;

  // Single file compilation
  if (inputPath.extension() == ".jack") {
    outputPath = fs::path(inputPath).stem();
    inputPaths = {inputPath};
  }

  // Directory compilation
  else if (fs::is_directory(inputPath)) {
    outputPath = inputPath.filename();
    if (outputPath == "")
      outputPath = fs::path(inputPath).parent_path().filename();

    inputPaths = getJackFilesInDirectory(inputPath);

    if (inputPaths.size() == 0) {
      std::cout << "Path contains no .jack files. Path must be a .jack file or "
                   "a directory containing at least one .jack file."
                << std::endl;
      exit(1);
    }
  }

  else {
    std::cout
        << "Invalid path passed to compiler. Path must be a .jack file or "
           "a directory containing at least one .jack file."
        << std::endl;
    exit(1);
  }

  outputPath += ".xml";

  for (fs::path inputPath : inputPaths) {
    const Tokens tokens = tokenizeJackFile(inputPath);
  }

  return 0;
}