#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace FSUtils
{
    typedef std::vector<fs::path> JackFiles;

    JackFiles getJackFilesInDirectory(const fs::path &directoryPath);
}