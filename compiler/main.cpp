#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "fs_utils.hpp"

int main(int, const char *argv[])
{
  if (!argv[1])
    throw std::invalid_argument("No input file given");

  const std::string inputPath{argv[1]};
  std::string outputFileName;

  // Create list of files to read
  std::vector<std::string> inputFiles;
  if (isDirectory(inputPath))
  {
    inputFiles = getInputFiles(inputPath);
    outputFileName = getDirectoryName(inputPath) + ".vm";
  }
  else if (isHackFile(inputPath))
  {
    inputFiles = {inputPath};
    outputFileName = getFileBaseName(inputPath) + ".vm";
  }
  else
  {
    throw std::invalid_argument(
        "Invalid path passed to compiler. Path must be a .hack file or "
        "directory containing .hack files.");
  }

  /*
  std::ofstream outputFile{outputFileName};

  outputFile.close();
  */

  return 0;
}