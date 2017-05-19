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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <memory>
#include <string>
#include <list>
#include <map>

#include "spdlog/spdlog.h"

namespace msns {

  class Config {
  protected:
    std::shared_ptr<spdlog::logger> logger;
    std::string fileName;

    Config() { 
      logger = spdlog::get("config");
      if (!logger) logger = spdlog::stdout_color_mt("config"); };
    explicit Config(const std::string& fileName) : Config() { this->fileName = fileName; };
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
