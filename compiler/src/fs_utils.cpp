#include "fs_utils.hpp"

#include <filesystem>
#include <iostream>

namespace FSUtils
{
  JackFiles getJackFilesInDirectory(const fs::path &directoryPath)
  {
    JackFiles jackFiles;

    for (const fs::directory_entry &path : fs::directory_iterator(directoryPath))
      if (path.path().extension() == ".jack")
        jackFiles.push_back(path.path());

    return jackFiles;
  }
}