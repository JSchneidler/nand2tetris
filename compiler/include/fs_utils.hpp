#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> getJackFilesInDirectory(const fs::path &directoryPath);

#endif