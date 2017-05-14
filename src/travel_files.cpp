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

#include <iostream>
#include <cstring>
#include <memory>

#include <boost/log/trivial.hpp>

#include "travel_files.hpp"
#include "config.hpp"

using namespace std;
using namespace msns;

string sysErrMsg();

void msns::travelDir(const string& path, file_size& size, list<Report>& reports){
  BOOST_LOG_TRIVIAL(trace) << "travelFile - Path: " << path;
  size = 0;

  shared_ptr<DIR> baseDir(opendir(path.c_str()), closedir);
  if (!baseDir){
    BOOST_LOG_TRIVIAL(error) << sysErrMsg() << " - Path: " << path;
    return;
  }

  unique_ptr<LocalConfig> localConfig(nullptr);
  bool makeReport = false;
  dirent* dir;

  while((dir = readdir(baseDir.get())) != NULL){
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    string dirPath = path + PATH_SEPARATOR + dir->d_name;

    if (dir->d_type == DT_REG && LOCAL_CONFIG_NAME == dir->d_name){
      BOOST_LOG_TRIVIAL(info) << "Found limited folder";
      makeReport = true;
      localConfig.reset(new LocalConfig(dirPath));
      localConfig->load();
    }
    
    if (dir->d_type == DT_REG){
      // It is a file
      long fileSize = getSize(dirPath);
      
      if (fileSize >= 0){
	size += fileSize;
	BOOST_LOG_TRIVIAL(trace) << "File: " << dirPath << " SIZE: " << fileSize;
      } else {
	BOOST_LOG_TRIVIAL(error) << sysErrMsg() << " File: " << fileSize;
      }

    } else if (dir->d_type == DT_DIR){
      // It is a directory
      file_size dirSize;
      travelDir(dirPath, dirSize, reports);
      size += dirSize;
    }
  }

  // Report
  if (makeReport){
    BOOST_LOG_TRIVIAL(trace) << "Making report";
    Report report;
    report.name = localConfig->name;
    report.path = path;
    report.sizeLimit = localConfig->sizeLimit;
    report.size = size / (1024 * 1024);
    report.localEmails = localConfig->emails;
    
    reports.push_back(report);
  }

  if (errno != 0)
    BOOST_LOG_TRIVIAL(error) << sysErrMsg() << " - Path: " << path;
  BOOST_LOG_TRIVIAL(trace) << "Path: " << path << " SIZE: " << size;
}

long msns::getSize(const string& path){
  struct stat buf;
  if (lstat(path.c_str(), &buf) != 0)
    return -1;  // errno has the error number
  return buf.st_size;
}

string msns::sysErrMsg(){
  string msg;
  switch (errno){
  case EACCES: msg = "Permission denied."; break;
  case ENOTDIR: msg = "Is not a directory"; break;
  case ENOENT: msg = "Directory does not exist"; break;
  case EBADF: msg = "Invalid directory stream descriptor"; break;
  case EMFILE: msg = "Too many open files"; break;
  default: msg = "Error No. " + to_string(errno);
  }
  return msg;
}
