#include <string>
#include <list>

#include "gtest/gtest.h"

#include "config.hpp"

using namespace msns;

const std::string ASSETS_PATH = "./test/assets/";

#define EXPECT_ITERABLE_EQ(ref, target){ \
  EXPECT_EQ(ref.size(), target.size()); \
  auto refIt = ref.begin(); \
  auto targetIt = target.begin(); \
  for (; refIt != ref.end() && targetIt != target.end(); ++refIt, ++targetIt) \
    EXPECT_EQ(*refIt, *targetIt); \
  }


TEST(GlobalConfigTest, CorretLoad){
  const std::string CONFIG = "global_config_correct_loading.ini";
  GlobalConfig config(ASSETS_PATH + CONFIG);
  config.load();

  // General
  EXPECT_EQ("machine_test", config.machineName);

  // SMTP
  EXPECT_EQ("smtp://smtp.gmail.com:587", config.emailUrl);
  EXPECT_EQ("email_user", config.emailUsername);
  EXPECT_EQ("1234", config.emailPassword);
  EXPECT_EQ(1, config.emailSsl);
  EXPECT_EQ("sender@test.com", config.emailFrom);


  std::list<std::string> expectedEmails = {"test@test.com", "test2@test.com"};
  EXPECT_ITERABLE_EQ(expectedEmails, config.emails);

  std::list<std::string> expectedFolders = {
    "/home/user/Documents", "/home/user/Downloads"};
  EXPECT_ITERABLE_EQ(expectedFolders, config.folders);
}

TEST(GlobalConfigTest, DisabledFolders){
  const std::string CONFIG = "global_config_disabled_folders.ini";
  GlobalConfig config(ASSETS_PATH + CONFIG);
  config.load();

  std::list<std::string> expectedFolders = {"/home/user/Downloads"};
  EXPECT_ITERABLE_EQ(expectedFolders, config.folders);
}

TEST(GlobalConfigTest, DisabledEmails){
  const std::string CONFIG = "global_config_disabled_emails.ini";
  GlobalConfig config(ASSETS_PATH + CONFIG);
  config.load();

  std::list<std::string> expectedEmails = {"test2@test.com"};
  EXPECT_ITERABLE_EQ(expectedEmails, config.emails);
}
