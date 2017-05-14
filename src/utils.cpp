#include <sys/types.h>
#include <sys/stat.h>

#include <boost/log/trivial.hpp>

#include <iomanip>
#include <algorithm>
#include <sstream>
#include <ctime>

#include "utils.hpp"

using namespace std;

vector<string> msns::split(string path, char delimiter){
  vector<string> out;
  auto start = path.begin();

  while (start != path.end()){
    auto end = find(start, path.end(), delimiter);
    out.push_back(string(start, end));

    start = end;
    if (start != path.end()) ++start;
  }
  
  return out;
}

string msns::formattedStr(const string& str, size_t dim, char c){
  if (str.size() >= dim)
    return str.substr(0, dim);
  string out(str);
  out.append(dim - str.size(), c);
  return out;
}

string msns::formattedStr(float fl, int prec, size_t dim, char c){
  ostringstream out;
  out << setprecision(prec) << fl;
  return formattedStr(out.str(), dim, c);
}

string msns::repeatChr(int dim, char c){
  string out;
  out.append(dim, c);
  return out;
}

string msns::currentTime(const string& format){
  time_t now = time(NULL);
  char buff[100];
  if (strftime(buff, sizeof(buff), format.c_str(), localtime(&now)))
    return string(buff);
  return "";
}

bool msns::validFolder(const string& path){
  struct stat buf;
  if (stat(path.c_str(), &buf) != 0)
    return false;
  if (!S_ISDIR(buf.st_mode))
    return false;
  return true;
}

bool msns::fileExists(const string& path){
  struct stat buf;
  if (stat(path.c_str(), &buf) != 0)
    return false;
  if (S_ISREG(buf.st_mode))
    return true;
  return false;
}
