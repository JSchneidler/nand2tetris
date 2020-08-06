#include "fs_utils.hpp"

#include <filesystem>
#include <iostream>

std::vector<fs::path> getJackFilesInDirectory(const fs::path &directoryPath) {
  std::vector<fs::path> jackFiles;

  for (const fs::directory_entry &path : fs::directory_iterator(directoryPath))
    if (path.path().extension() == ".jack") jackFiles.push_back(path.path());

  return jackFiles;
}