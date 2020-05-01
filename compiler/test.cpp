#include "fs_utils.hpp"

int fail(const std::string &reason);

int fsUtilsTest()
{
  // isDirectory(path)
  const std::string paths[] = {"test", "test/", "./test", "./test/"};
  for (std::string path : paths)
  {
    if (!isDirectory(path))
      return fail("isDirectory should return true for path: " + path);
  }

  // isHackFile(path)
  // getDirectoryName(path)
  // getFileBaseName(path)
  // getInputFiles(path)
  // readDirectory(path)

  return 0;
}

int main()
{
  if (fsUtilsTest())
    return 1;

  printf("Success");
  return 0;
}

int fail(const std::string &reason)
{
  printf("%s\n", reason.c_str());
  return 1;
}