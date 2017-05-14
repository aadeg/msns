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

#ifndef _CONST_H_
#define _CONST_H_
#include <string>

namespace msns {
  const std::string VERSION_MSG = "msns - Version " + std::string(VERSION) + "\n" +
    "msns Copyright (C) 2017 Andrea Giove\n" +
    "License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl.html>\n" +
    "This program comes with ABSOLUTELY NO WARRANTY";
  const std::string LOCAL_CONFIG_NAME = "msns.ini";
  const char PATH_SEPARATOR = '/';
}

#endif
