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
#include <algorithm>

#include "fmt/format.h"

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
				     const vector<report_ptr>& reports) const {
  string str("MSNS REPORT\n\n");
  str += fmt::format("    {:<15}{}\n", "Date:", currentTime("%a %d %b %Y %H:%M"));
  str += fmt::format("    {:<15}{}\n", "Rep. Type:", reportLvl);
  str += "\n";

  str += "MONITOR REPORT\n\n";
  str += fmt::format("    {:<20} {:<60} {:>10} {:>10} {:>8}\n",
		     "name", "path", "dim (MB)", "limit (MB)", "perc");
  str += fmt::format("    {:-<20} {:-<60} {:->10} {:->10} {:->8}\n",
		     "", "", "", "", "", "");
  
  for (auto r : reports){
    double perc = int(r->size - r->sizeLimit) / double(r->sizeLimit) * 100;
    str += fmt::format("    {:<20} {:<60} {:>10d} {:>10d} {:>7.2f}%\n",
		       r->name, r->path, r->size, r->sizeLimit, perc);
  }

  if (reports.empty())
    str += fmt::format("    No reports\n");

  str += "\n\n" + EMAIL_FOOTER;
  out << str;
  return out;
}

void EmailBuilder::sendAll(EmailHandler& sender, const string& from) const {
  string subj = "[msns] Report " + machineName;

  // Global Emails
  ostringstream gStream;
  outputMessage(gStream, globalReports);
  string globalMsg = gStream.str();
  for(email_addr email : globalEmails){
    vector<string> to = {email};
    sender.sendEmail(from, to, subj, globalMsg);
  }

  // Local Emails
  for(auto kv : emailMap){
      ostringstream lStream;
      outputMessage(lStream, kv.second);
      string localMsg = lStream.str();
      vector<string> to = {kv.first};
      sender.sendEmail(from, to, subj, localMsg);
  }
    
}




