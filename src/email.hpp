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

#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <curl/curl.h>

#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include "spdlog/spdlog.h"

#include "const.hpp"
#include "report.hpp"

namespace msns {
  class EmailHandler {
  protected:
    std::shared_ptr<spdlog::logger> logger;
  public:
    EmailHandler() { logger = spdlog::get(MSNS_LOGGER); };
    virtual void sendEmail(const std::string& from,
			   const std::vector<std::string>& to,
			   const std::string& subject,
			   const std::string& body) = 0;
  };

  class CurlEmail : public EmailHandler {
    std::string url;
    std::string username;
    std::string password;
    bool sslReqd;

    std::shared_ptr<CURL> getCurl(const std::string& from,
				  std::shared_ptr<curl_slist> recipents) const;
    static std::shared_ptr<curl_slist> getRecipents (const std::vector<std::string>& to);
    static std::shared_ptr<std::stringstream> getBodyStream
    (const std::string& from, const std::vector<std::string>& to,
     const std::string& subject, const std::string& body);
  protected:
    static size_t body_payload(char *buffer, size_t size, size_t nitems, void *instream);
  public:
    CurlEmail(const std::string& url, const std::string& username,
	      const std::string& password, bool sslReqd = true);
    CurlEmail(const CurlEmail& other);
    CurlEmail(CurlEmail&& other);
    ~CurlEmail() {};

    CurlEmail& operator=(const CurlEmail& other);
    CurlEmail& operator=(CurlEmail&& other);

    void sendEmail(const std::string& from,
		   const std::vector<std::string>& to,
		   const std::string& subject,
		   const std::string& body);
  };

  class EmailBuilder {
  public:
    typedef std::string email_addr;
    typedef std::shared_ptr<const Report> report_ptr;

  private:
    std::string machineName;
    std::string reportLvl;
    std::vector<email_addr> globalEmails;
    std::vector<report_ptr> globalReports;
    std::map<email_addr, std::vector<report_ptr>> emailMap;

    std::ostream& outputMessage(std::ostream& out, 
				const std::vector<report_ptr>& reports) const;
  public:
    EmailBuilder(const std::string& machineName,
		 std::vector<email_addr> globalEmails,
		 const std::string& reportLvl) :
      machineName(machineName), reportLvl(reportLvl),
      globalEmails(globalEmails) {};
    
    void addReport(const Report& report);
    void sendAll(EmailHandler& sender, const std::string& from) const;
  };
}

#endif
