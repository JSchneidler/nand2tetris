#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <string>
#include <vector>

bool isDirectory(const std::string &path);
bool isHackFile(const std::string &path);
std::string getDirectoryName(const std::string &directoryPath);
std::string getFileBaseName(const std::string &filePath);
std::vector<std::string> getInputFiles(const std::string &path);
std::vector<std::string> readDirectory(const std::string &inputPath);

#endif