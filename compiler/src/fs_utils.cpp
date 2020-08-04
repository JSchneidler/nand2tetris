#include "fs_utils.hpp"

#include <filesystem>

std::vector<std::filesystem::path> getJackFilesInDirectory(
    const std::filesystem::path &directoryPath) {
  std::vector<std::filesystem::path> jackFiles;
  /*
  DIR *dir;
  struct dirent *ent;

  dir = opendir(inputPath.c_str());
  if (dir != NULL) {
    while ((ent = readdir(dir)) != NULL)
      if (ent->d_type == DT_REG && isJackFile(ent->d_name))
        jackFiles.push_back(ent->d_name);
  } else {
    fprintf(stderr, "Cannot open %s\n", inputPath.c_str());
    exit(EXIT_FAILURE);
  }

  closedir(dir);

  */
  return jackFiles;
}