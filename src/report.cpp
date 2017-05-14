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

#include "report.hpp"

using namespace std;
using namespace msns;

ostream& operator<<(ostream& os, const Report& r){
  double perc = (r.size - r.sizeLimit) / r.sizeLimit * 100;
  os << "Name: " << r.name << endl
     << "Path: " << r.path << endl
     << "Size: " << r.size << endl
     << "SLmt: " << r.sizeLimit << endl
     << "Perc: " << perc << "%" << endl;
  return os;
}
