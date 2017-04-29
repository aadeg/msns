#include <iostream>
#include <string>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "config.hpp"
#include "travel_files.hpp"
#include "email.hpp"

using namespace std;
using namespace msns;

namespace msns {
  const string LOCAL_CONFIG_NAME = ".msns";
}

int main(int argc, char* argv[]){


  /*
  string psw = "06bd04b9f2616ac3793d41217b345682f63b381e";
  CurlEmail mail("smtp://smtp.sparkpostmail.com:587",
		  "SMTP_Injection", psw,
		  true);

  string from = "noreply@blindrage.ovh";
  list<string> to = {"andreagiove@outlook.com"};
  string subject = "Messaggio di prova CURL";
  string body = "Body messaggio\nSeconda Riga\nTerza";

  mail.sendEmail(from, to, subject, body);
  */
  return 0;

}
