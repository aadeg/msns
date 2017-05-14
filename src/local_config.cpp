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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/optional.hpp>

namespace pt = boost::property_tree;

using namespace std;
using namespace msns;

LocalConfig::LocalConfig() {}

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

void LocalConfig::save(const string& fileName) const {
  pt::ptree tree;
  
  tree.put("general.name", name);
  tree.put("general.size_limit", sizeLimit);

  pt::ptree emailTree;
  for (auto& email : emails)
    emailTree.push_back(pt::ptree::value_type(email, pt::ptree("1")));
  if (emails.empty())
    emailTree.push_back(pt::ptree::value_type("test@test.com", pt::ptree("0")));

  tree.put_child("emails", emailTree);
  pt::write_ini(fileName, tree);
}
