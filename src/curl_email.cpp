#include <boost/log/trivial.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <iterator>

#include "email.hpp"

using namespace std;
using namespace msns;

CurlEmail::CurlEmail(const string& url, const string& username,
		     const string& password, bool sslReqd) :
  url(url), username(username), password(password), sslReqd(sslReqd){}

CurlEmail::CurlEmail(const CurlEmail& other) :
  url(other.url), username(other.username), password(other.password),
  sslReqd(other.sslReqd) {}

CurlEmail::CurlEmail(CurlEmail&& other) :
  url(other.url), username(other.username), password(other.password),
  sslReqd(other.sslReqd) {}

CurlEmail& CurlEmail::operator= (const CurlEmail& other){
  url = other.url;
  username = other.username;
  password = other.password;
  sslReqd = sslReqd;
  return *this;
}

CurlEmail& CurlEmail::operator= (CurlEmail&& other){
  url = move(other.url);
  username = move(other.username);
  password = move(other.password);
  sslReqd = sslReqd;
  return *this;
}

void CurlEmail::sendEmail(const string& from, const list<string>& to,
			  const string& subject, const string& body){

  shared_ptr<curl_slist> recipents = CurlEmail::getRecipents(to);
  shared_ptr<CURL> curl = CurlEmail::getCurl(from, recipents);

  
  shared_ptr<stringstream> sstream = CurlEmail::getBodyStream(from, to, subject, body);

  curl_easy_setopt(curl.get(), CURLOPT_VERBOSE, 1);
  curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, CurlEmail::body_payload);
  curl_easy_setopt(curl.get(), CURLOPT_READDATA, sstream.get());
  curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 1L);

  CURLcode res = curl_easy_perform(curl.get());
  if (res != CURLE_OK)
    BOOST_LOG_TRIVIAL(error) << "curl_easy_perform failed: "
			     << curl_easy_strerror(res);
}

shared_ptr<CURL> CurlEmail::getCurl(const string& from,
				    shared_ptr<curl_slist> recipents) const {
  shared_ptr<CURL> curl(curl_easy_init(), curl_easy_cleanup);

  // Set Options
  curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);

  curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl.get(), CURLOPT_MAIL_FROM, from.c_str());
  curl_easy_setopt(curl.get(), CURLOPT_MAIL_RCPT, recipents.get());
  curl_easy_setopt(curl.get(), CURLOPT_USERNAME, username.c_str());
  curl_easy_setopt(curl.get(), CURLOPT_PASSWORD, password.c_str());
   
  if (sslReqd)
    curl_easy_setopt(curl.get(), CURLOPT_USE_SSL, CURLUSESSL_ALL);

  return curl;
}


shared_ptr<curl_slist> CurlEmail::getRecipents(const list<string>& to) {
  curl_slist* tmp = NULL;
  for (auto it = to.cbegin(); it != to.end(); ++it)
    tmp = curl_slist_append(tmp, it->c_str());

  shared_ptr<curl_slist> slist(tmp, [](curl_slist* slist){
      if (slist != nullptr) curl_slist_free_all(slist);
    });
  return slist;
}

shared_ptr<stringstream> CurlEmail::getBodyStream(const std::string& from,
				      const std::list<std::string>& to,
				      const std::string& subject,
				      const std::string& body){
  stringstream* sstream = new stringstream(ios_base::in | ios_base::out);

  BOOST_LOG_TRIVIAL(debug) << body;
  string _body = boost::replace_all_copy(body, "\n", "\r\n");

  *sstream << "From: <" << from << ">\r\n";
  for (auto it = to.cbegin(); it != to.end(); ++it)
    *sstream << "To: <" << (*it) << ">\r\n";
  *sstream << "Subject: " << subject << "\r\n"
	  << "\r\n"
	   << _body << "\r\n";

  BOOST_LOG_TRIVIAL(debug) << sstream->str();
  return shared_ptr<stringstream>(sstream);
}

size_t CurlEmail::body_payload(char *buffer, size_t size, size_t nitems, void *instream){
  stringstream& sstream = *((stringstream*) instream);

  if(!sstream || size == 0 || nitems == 0 || (size * nitems) < 1)
    return 0;
  sstream.read(buffer, size * nitems);

  return sstream.gcount();
}
