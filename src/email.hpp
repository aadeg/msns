#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <curl/curl.h>

#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <map>

#include "report.hpp"

namespace msns {
  class EmailHandler {
  public:
    virtual void sendEmail(const std::string& from,
			   const std::list<std::string>& to,
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
    static std::shared_ptr<curl_slist> getRecipents (const std::list<std::string>& to);
    static std::shared_ptr<std::stringstream> getBodyStream
    (const std::string& from, const std::list<std::string>& to,
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
		   const std::list<std::string>& to,
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
    std::list<email_addr> globalEmails;
    std::list<report_ptr> globalReports;
    std::map<email_addr, std::list<report_ptr>> emailMap;

    std::ostream& outputMessage(std::ostream& out, 
				const std::list<report_ptr>& reports) const;
  public:
    explicit EmailBuilder(const std::string& machineName,
			  std::list<email_addr> globalEmails,
			  const std::string& reportLvl) :
      machineName(machineName), reportLvl(reportLvl),
      globalEmails(globalEmails) {};

    void addReport(const Report& report);
    void sendAll(EmailHandler& sender, const std::string& from) const;
    void debug() const;
  };
}

#endif
