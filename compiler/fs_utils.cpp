#include "fs_utils.hpp"
#include <dirent.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool isDirectory(const std::string &path)
{
  return fs::is_directory(path);
}

bool isHackFile(const std::string &path)
{
  return (fs::is_regular_file(path) &&
          path.substr(path.length() - 5) == ".hack");
}

std::string getDirectoryName(const std::string &directoryPath)
{
  return directoryPath;
}
std::string getFileBaseName(const std::string &filePath) { return filePath; }

std::vector<std::string> getInputFiles(const std::string &path)
{
  std::vector<std::string> inputFiles{readDirectory(path)};

  return inputFiles;
}

std::vector<std::string> readDirectory(const std::string &inputPath)
{
  DIR *dir;
  struct dirent *ent;
  std::vector<std::string> vmFiles;

  dir = opendir(inputPath.c_str());
  if (dir != NULL)
  {
    while ((ent = readdir(dir)) != NULL)
      if (ent->d_type == DT_REG && isHackFile(ent->d_name))
        vmFiles.push_back(ent->d_name);
  }
  else
  {
    fprintf(stderr, "Cannot open %s\n", inputPath.c_str());
    exit(EXIT_FAILURE);
  }

  closedir(dir);

  return vmFiles;
}