#include "config.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/optional.hpp>

namespace pt = boost::property_tree;

using namespace std;
using namespace msns;

LocalConfig::LocalConfig(const LocalConfig& other) :
  Config(other), sizeLimit(other.sizeLimit), name(other.name),
  emails(other.emails) {}

LocalConfig::LocalConfig(LocalConfig&& other) :
  Config(other), sizeLimit(other.sizeLimit), name(other.name),
  emails(other.emails) {}

LocalConfig& LocalConfig::operator= (const LocalConfig& other) {
  Config::operator=(other);

  sizeLimit = other.sizeLimit;
  name = other.name;
  emails = other.emails;

  return *this;
}


LocalConfig& LocalConfig::operator= (LocalConfig&& other){
  Config::operator=(other);
  
  sizeLimit = other.sizeLimit;
  name = move(other.name);
  emails = move(other.emails);

  return *this;
}

void LocalConfig::load(){
  pt::ptree tree;
  pt::read_ini(fileName, tree);

  name = tree.get<string>("general.name");
  sizeLimit = tree.get<int>("general.size_limit");

  auto optEmails = tree.get_child_optional("emails");
  if (optEmails){
    for (auto& key : *optEmails){
      if (key.second.get_value<bool>())
	emails.push_back(key.first);
    }
  }
}
