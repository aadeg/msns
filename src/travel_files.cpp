#include <iostream>
#include <cstring>

#include <boost/log/trivial.hpp>

#include "travel_files.hpp"

using namespace std;
using namespace msns;

string errMsg(){
  string msg;
  switch(errno){
  case EACCES: msg = "Permission denied."; break;
  case ENOTDIR: msg = "Is not a directory"; break;
  case ENOENT: msg = "Directory does not exist"; break;
  case EBADF: msg = "Invalid directory stream descriptor"; break;
  default: msg = "Error No. " + errno;
  }
  return msg;
}

void msns::travelDir(const string& path, int& size){
  BOOST_LOG_TRIVIAL(trace) << "travelFile - Path: " << path;
  size = 0;

  DIR* baseDir = opendir(path.c_str());
  if (!baseDir){
    string msg = errMsg();
    BOOST_LOG_TRIVIAL(error) << errMsg() << " - Path: " << path;
    return;
  }

  dirent* dir;
  while((dir = readdir(baseDir)) != NULL){
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    if (LOCAL_CONFIG_NAME == dir->d_name){
      // TODO 
    }
    
    string dirPath = path + PATH_SEPARATOR + dir->d_name;
    if (dir->d_type == DT_REG){
      // It is a file
      int fileSize = getSize(dirPath);
      
      if (fileSize > 0){
	size += fileSize;
	BOOST_LOG_TRIVIAL(trace) << "File: " << dirPath << " SIZE: " << fileSize;
      } else {
	BOOST_LOG_TRIVIAL(error) << errMsg() << "File: " << fileSize;
      }

    } else if (dir->d_type == DT_DIR){
	// It is a directory
	int dirSize;
	travelDir(dirPath, dirSize);
	size += dirSize;
    }
  }

  if (errno != 0)
    BOOST_LOG_TRIVIAL(error) << errMsg() << " - Path: " << path;
  BOOST_LOG_TRIVIAL(trace) << "Path: " << path << " SIZE: " << size;
}

int msns::getSize(const string& path){
  struct stat buf;
  if (lstat(path.c_str(), &buf) != 0)
    return -1;  // errno has the error number
  return buf.st_size;
}
