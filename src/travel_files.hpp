#ifndef _TRAVEL_FILES_H_
#define _TRAVEL_FILES_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <string>

namespace msns {
  extern const std::string LOCAL_CONFIG_NAME;
  const char PATH_SEPARATOR = '/';

  void travelDir(const std::string& path, int& size);
  int getSize(const std::string& path);
}

#endif
