#ifndef _TRAVEL_FILES_H_
#define _TRAVEL_FILES_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <string>
#include <list>

#include "const.hpp"
#include "report.hpp"

namespace msns {
  typedef unsigned long file_size;

  void travelDir(const std::string& path, file_size& size, std::list<msns::Report>& reports);
  long getSize(const std::string& path);

  std::string sysErrMsg();
}

#endif
