#include "email.hpp"

using namespace std;
using namespace msns;

EmailHandler::EmailHandler(){
  logger = spdlog::get("email");
  if (!logger)
    logger = spdlog::stdout_color_mt("email");
}
