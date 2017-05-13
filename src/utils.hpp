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
}


#endif
