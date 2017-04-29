#include "config.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using namespace msns;

GlobalConfig::GlobalConfig(const GlobalConfig& other) :
  Config(other), folders(other.folders), emails(other.emails) {}

GlobalConfig::GlobalConfig(GlobalConfig&& other) :
  Config(other), folders(other.folders), emails(other.emails) {}

GlobalConfig& GlobalConfig::operator= (const GlobalConfig& other) {
  Config::operator=(other);

  folders = other.folders;
  emails = other.emails;
  return *this;
}


GlobalConfig& GlobalConfig::operator= (GlobalConfig&& other){
  Config::operator=(other);
  
  folders = move(other.folders);
  emails = move(other.emails);
  return *this;
}

void GlobalConfig::load(){
}
