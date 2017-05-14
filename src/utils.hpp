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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

namespace msns {
  extern const char PATH_SEPARATOR;

  std::vector<std::string> split(std::string path, char delimiter);
  bool detect_nested_path(boost::property_tree::ptree& tree, std::string path);
  bool detect_nested_path(boost::property_tree::ptree& tree,
			  std::vector<std::string>::iterator l_start,
			  std::vector<std::string>::iterator l_end);


  std::string formattedStr(const std::string& str, std::size_t dim, char c = ' ');
  std::string formattedStr(float fl, int prec, std::size_t dim, char c = ' ');
  std::string repeatChr(int dim, char c = ' ');

  std::string currentTime(const std::string& format);

  bool validFolder(const std::string& path);
  bool fileExists(const std::string& filePath);
}


#endif
