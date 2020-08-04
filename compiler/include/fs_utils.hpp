#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <filesystem>
#include <string>
#include <vector>

std::vector<std::filesystem::path> getJackFilesInDirectory(
    const std::filesystem::path &directoryPath);

#endif