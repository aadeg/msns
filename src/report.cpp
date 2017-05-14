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
