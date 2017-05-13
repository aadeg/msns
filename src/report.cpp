#include "report.hpp"

using namespace std;
using namespace msns;

ostream& operator<<(ostream& os, const Report& r){
  float perc = (r.size - r.sizeLimit) * 100 / r.sizeLimit;
  os << "Name: " << r.name << endl
     << "Path: " << r.path << endl
     << "Size: " << r.size << endl
     << "SLmt: " << r.sizeLimit << endl
     << "Perc: " << perc << "%" << endl;
  return os;
}
