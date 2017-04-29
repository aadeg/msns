#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <list>

namespace msns {

  class Config {
  protected:
    std::string fileName;

    Config(const std::string& fileName) : fileName(fileName) {};
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
    std::list<std::string> folders;
    std::list<std::string> emails;

  public:
    GlobalConfig(const std::string& fileName) : Config(fileName) {};
    GlobalConfig(const GlobalConfig& other);
    GlobalConfig(GlobalConfig&& other);
    virtual ~GlobalConfig() {};

    GlobalConfig& operator= (const GlobalConfig& other);
    GlobalConfig& operator= (GlobalConfig&& other);
    
    void load();
  };
  
  class LocalConfig : public Config {
    int sizeLimit;
    std::list<std::string> emails;

  public:
    LocalConfig(const std::string& fileName) : Config(fileName) {};
    LocalConfig(const LocalConfig& other);
    LocalConfig(LocalConfig&& other);
    virtual ~LocalConfig() {};

    LocalConfig& operator= (const LocalConfig& other);
    LocalConfig& operator= (LocalConfig&& other);
    
    void load();
  };

}

#endif
