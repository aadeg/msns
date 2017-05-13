#ifndef _CONST_H_
#define _CONST_H_
#include <string>

namespace msns {
  const std::string VERSION_MSG = "msns - Version " + std::string(VERSION) + "\n" +
    "Copyright (C) 2017 Giove Andrea\n" +
    "License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl.html>\n" +
    "This is free software; you are free to change and redistribute it.\n" +
    "There is NO WARRANTY, to the extent permitted by law.";
  const std::string LOCAL_CONFIG_NAME = "msns.ini";
  const char PATH_SEPARATOR = '/';
}

#endif
