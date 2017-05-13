#include "config.hpp"
#include "utils.hpp"

#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace pt = boost::property_tree;

using namespace std;
using namespace msns;

  std::string emailUrl;
    std::string emailUsername;
    std::string emailPassword;
    bool emailSsl;
    std::string emailFrom;

GlobalConfig::GlobalConfig(const GlobalConfig& other) :
  Config(other), machineName(other.machineName), 
  folders(other.folders), emails(other.emails),
  emailUrl(other.emailUrl), emailUsername(other.emailUsername),
  emailPassword(other.emailPassword), emailSsl(other.emailSsl),
  emailFrom(other.emailFrom){}

GlobalConfig::GlobalConfig(GlobalConfig&& other) :
  Config(other), machineName(other.machineName), 
  folders(other.folders), emails(other.emails),
  emailUrl(other.emailUrl), emailUsername(other.emailUsername),
  emailPassword(other.emailPassword), emailSsl(other.emailSsl),
  emailFrom(other.emailFrom){}

GlobalConfig& GlobalConfig::operator= (const GlobalConfig& other) {
  Config::operator=(other);

  machineName = other.machineName;
  folders = other.folders;
  emails = other.emails;

  emailUrl = other.emailUrl;
  emailUsername = other.emailUsername;
  emailPassword = other.emailPassword;
  emailSsl = other.emailSsl;
  emailFrom = other.emailFrom;

  return *this;
}


GlobalConfig& GlobalConfig::operator= (GlobalConfig&& other){
  Config::operator=(other);
  
  machineName = move(other.machineName);
  folders = move(other.folders);
  emails = move(other.emails);

  emailUrl = move(other.emailUrl);
  emailUsername = move(other.emailUsername);
  emailPassword = move(other.emailUsername);
  emailPassword = move(other.emailPassword);
  emailSsl = move(other.emailSsl);
  emailFrom = move(other.emailFrom);

  return *this;
}

void GlobalConfig::load(){
  pt::ptree tree;
  pt::read_ini(fileName, tree);

  machineName = tree.get<string>("general.machine_name");

  for (auto& key : tree.get_child("folders")){
    if (key.second.get_value<bool>())
      folders.push_back(key.first);
  }

  for (auto& key : tree.get_child("emails")){
    if (key.second.get_value<bool>())
      emails.push_back(key.first);
  }

  emailUrl = tree.get<string>("smtp.url");
  emailUsername = tree.get<string>("smtp.username");
  emailPassword = tree.get<string>("smtp.password");
  emailSsl = tree.get<bool>("smtp.ssl");
  emailFrom = tree.get<string>("smtp.from");

  sanitize();
}

void GlobalConfig::sanitize(){
  auto it = folders.begin();
  while (it != folders.end()){
    if (it->front() != '/'){
      BOOST_LOG_TRIVIAL(warning) << "No-absolute path found: " << *it;
      BOOST_LOG_TRIVIAL(warning) << "Path must be absolute";
      it = folders.erase(it);
    } else if (it->back() == '/'){
      (*it) = it->substr(0, it->size() - 1);
    }

    ++it;
  }
  for (auto it = folders.begin(); it != folders.end(); ++it){
   
   }
}
