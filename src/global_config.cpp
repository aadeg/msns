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

#include "config.hpp"
#include "utils.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace pt = boost::property_tree;

using namespace std;
using namespace msns;

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
      logger->error("Non-absolute path found: {}. Path must be absolute", *it);
      it = folders.erase(it);
    } else if (it->back() == '/'){
      (*it) = it->substr(0, it->size() - 1);
    }

    ++it;
  }
  for (auto it = folders.begin(); it != folders.end(); ++it){
   
   }
}
