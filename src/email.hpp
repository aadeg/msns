#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <curl/curl.h>

#include <memory>
#include <string>
#include <sstream>
#include <list>

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
}

#endif
