#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <list>
#include <map>

namespace msns {

  class Config {
  protected:
    std::string fileName;

    Config() {};
    explicit Config(const std::string& fileName) : fileName(fileName) {};
    Config(std::string&& fileName) : fileName(fileName) {};
    Config(const Config& other) : fileName(other.fileName) {};
    Config(Config&& other) { fileName = std::move(other.fileName); };
    virtual ~Config() {};

    virtual Config& operator= (const Config& other)
    { fileName = other.fileName; return *this; }
    virtual Config& operator= (Config&& other)
    { fileName = std::move(other.fileName); return *this;}
    
    virtual void load() = 0;
  };

  class GlobalConfig : public Config {
    void sanitize();
  public:
    std::string machineName;
    std::list<std::string> folders;
    std::list<std::string> emails;
    
    std::string emailUrl;
    std::string emailUsername;
    std::string emailPassword;
    bool emailSsl;
    std::string emailFrom;

    explicit GlobalConfig(const std::string& fileName) : Config(fileName) {};
    GlobalConfig(std::string&& fileName) : Config(fileName) {};
    GlobalConfig(const GlobalConfig& other);
    GlobalConfig(GlobalConfig&& other);
    virtual ~GlobalConfig() {};

    GlobalConfig& operator= (const GlobalConfig& other);
    GlobalConfig& operator= (GlobalConfig&& other);
    
    void load();
  };
  
  class LocalConfig : public Config {
  public:
    int sizeLimit;
    std::string name;
    std::list<std::string> emails;

    LocalConfig();
    explicit LocalConfig(const std::string& fileName) : Config(fileName) {};
    LocalConfig(const LocalConfig& other);
    LocalConfig(LocalConfig&& other);
    virtual ~LocalConfig() {};

    LocalConfig& operator= (const LocalConfig& other);
    LocalConfig& operator= (LocalConfig&& other);
    
    void load();
    void save(const std::string& fileName) const;
  };

}

#endif
