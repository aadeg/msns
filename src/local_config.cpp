#include "config.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using namespace msns;

LocalConfig::LocalConfig(const LocalConfig& other) :
  Config(other), sizeLimit(other.sizeLimit), emails(other.emails) {}

LocalConfig::LocalConfig(LocalConfig&& other) :
  Config(other), sizeLimit(other.sizeLimit), emails(other.emails) {}

LocalConfig& LocalConfig::operator= (const LocalConfig& other) {
  Config::operator=(other);

  sizeLimit = other.sizeLimit;
  emails = other.emails;

  return *this;
}


LocalConfig& LocalConfig::operator= (LocalConfig&& other){
  Config::operator=(other);
  
  sizeLimit = other.sizeLimit;
  emails = move(other.emails);

  return *this;
}

void LocalConfig::load(){
}
