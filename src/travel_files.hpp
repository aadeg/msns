//  msns - Max Size Notification System
//  Copyright (C) 2017  Andrea Giove
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
