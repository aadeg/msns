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

#include <boost/log/trivial.hpp>

#include <algorithm>

#include "const.hpp"
#include "utils.hpp"

namespace pt = boost::property_tree;
using namespace std;
using namespace msns;

// TODO

bool msns::detect_nested_path(pt::ptree& tree, string path){
  if (path.size() <= 0 || path[0] != PATH_SEPARATOR){
    BOOST_LOG_TRIVIAL(error) << "detect_nested_path - Invalid path!";
    throw;
  }

  path = path.substr(1, path.size());
  vector<string> splittedPath = split(path, PATH_SEPARATOR);
  return detect_nested_path(tree,
			    splittedPath.begin(), splittedPath.end());
}

bool msns::detect_nested_path(pt::ptree& tree, 
			      vector<string>::iterator l_start, vector<string>::iterator l_end){
  if (l_start == l_end){
    BOOST_LOG_TRIVIAL(trace) << "l_start == l_end";
    return false;
  }

  BOOST_LOG_TRIVIAL(trace) << "New call: " << *l_start;

  auto it = tree.find(*l_start);

  if (it == tree.not_found())
    BOOST_LOG_TRIVIAL(debug) << "Not found";
  else 
    BOOST_LOG_TRIVIAL(debug) << "Found";
  if (it != tree.not_found() && it->second.empty()){
    BOOST_LOG_TRIVIAL(debug) << "Nested detected";
    return true;
  } 

  pt::ptree newTree;  
  pt::ptree::value_type newNode(*l_start, newTree);
  if (it == tree.not_found())
    tree.push_front(newNode);
  else
    tree.insert(tree.to_iterator(it), newNode);

  return detect_nested_path(newTree, l_start + 1, l_end);
}
