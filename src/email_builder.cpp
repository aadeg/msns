#include <boost/log/trivial.hpp>

#include <algorithm>

#include "utils.hpp"
#include "email.hpp"

using namespace std;
using namespace msns;

void EmailBuilder::addReport(const Report& report){
  report_ptr rptr(&report, [](const Report*){});

  globalReports.push_back(rptr);
  for (email_addr email : rptr->localEmails)
    if (find(globalEmails.begin(), globalEmails.end(), email) == globalEmails.end())
      emailMap[email].push_back(rptr);
}

ostream& EmailBuilder::outputMessage(ostream& out,
				     const list<report_ptr>& reports) const {
  out << "MSNS REPORT" << endl
      << repeatChr(4)
      << formattedStr("Data:", 15)
      << currentTime("%a %d %b %Y %H:%M") << endl
      << repeatChr(4)
      << formattedStr("Tipo Rep.:", 15)
      << reportLvl << endl << endl;
  out << "RAPPORTO CONTROLLO"
      << endl << endl;
  out << repeatChr(4)
      << formattedStr("nome", 20) << " "
      << formattedStr("percorso", 60) << " "
      << formattedStr("dim (MB)", 10) << " "
      << formattedStr("limit (MB)", 10) << " "
      << formattedStr("perc", 8) << endl;
  out << repeatChr(4)
      << repeatChr(20, '-') << " "
      << repeatChr(60, '-') << " "
      << repeatChr(10, '-') << " "
      << repeatChr(10, '-') << " "
      << repeatChr(8, '-') << endl;

  for (auto r : reports){
    float perc = (r->size - r->sizeLimit) * 100 / r->sizeLimit;
    out  << repeatChr(4) 
	 << formattedStr(r->name, 20) << " "
	 << formattedStr(r->path, 60) << " "
	 << formattedStr(to_string(r->size), 10) << " "
	 << formattedStr(to_string(r->sizeLimit), 10) << " "
	 << formattedStr(perc, 2, 7) << '%' << endl;
  }

  if (reports.empty())
    out << repeatChr(4) << "Nessun rapporto" << endl;

  out << endl;

  return out;
}

void EmailBuilder::debug() const {
  for (auto kv : emailMap){
    BOOST_LOG_TRIVIAL(debug) << "Email: " << kv.first << " Reports";
    for (report_ptr rptr : kv.second)
      BOOST_LOG_TRIVIAL(debug) << " - " << rptr->name;
  }

  ostringstream out;
  outputMessage(out, globalReports);
  cout << out.str();
  
}

void EmailBuilder::sendAll(EmailHandler& sender, const string& from) const {
  string subj = "[msns] Rapporto";

  // Global Emails
  ostringstream gStream;
  outputMessage(gStream, globalReports);
  string globalMsg = gStream.str();
  for(email_addr email : globalEmails){
    list<string> to = {email};
    sender.sendEmail(from, to, subj, globalMsg);
  }

  // Local Emails
  for(auto kv : emailMap){
      ostringstream lStream;
      outputMessage(lStream, kv.second);
      string localMsg = lStream.str();
      list<string> to = {kv.first};
      sender.sendEmail(from, to, subj, localMsg);
  }
    
}




