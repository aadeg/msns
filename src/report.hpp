#ifndef _REPORT_H_
#define _REPORT_H_

#include <string>
#include <list>
#include <iostream>

namespace msns {
  struct Report {
    std::string name;
    std::string path;
    unsigned long sizeLimit;
    unsigned long size;
    std::list<std::string> localEmails;
  };
  
}

std::ostream& operator<<(std::ostream& os, const msns::Report& r);

#endif
