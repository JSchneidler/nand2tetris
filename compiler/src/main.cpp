#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "fs_utils.hpp"

int main(int, const char *argv[]) {
  if (!argv[1]) {
    std::cout << "No input file given. Terminating." << std::endl;
    return 1;
  }

  const std::filesystem::path inputPath{argv[1]};
  std::string outputFileName;

  if (!std::filesystem::exists(inputPath)) {
    std::cout << "Input path does not exist. Terminating." << std::endl;
    return 1;
  }

  // Create list of files to read
  std::vector<std::filesystem::path> inputFiles;

  // Single file compilation
  if (inputPath.extension() == ".jack") {
    outputFileName = std::filesystem::path(inputPath).stem().string() + ".vm";
    inputFiles = {inputPath};
  }

  // Directory compilation
  else if (std::filesystem::is_directory(inputPath)) {
    outputFileName = inputPath.filename().string() + ".vm";
    if (outputFileName == ".vm")
      outputFileName =
          std::filesystem::path(inputPath).parent_path().filename().string() +
          ".vm";

    inputFiles = getJackFilesInDirectory(inputPath);
  }

  // TODO: Multiple file compilation

  else {
    std::cout
        << "Invalid path passed to compiler. Path must be a .jack file or "
           "a directory containing at least one .jack file."
        << std::endl;
    return 1;
  }

  std::cout << outputFileName << std::endl;

  /*
  std::ofstream outputFile{outputFileName};

  outputFile.close();
  */

  return 0;
}