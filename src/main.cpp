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

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <memory>
#include <chrono>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "const.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "explore_files.hpp"
#include "email.hpp"
#include "report.hpp"

using namespace std;
using namespace msns;

void runAnalysis(const GlobalConfig& config, const string& reportLvl, bool emailNtf);
void initFolder(const string& initFolder, const string& initName, int initSize);

void initLogger(){
  using namespace spdlog;

  spdlog::set_level(level::debug);

  // SINKS
  vector<sink_ptr> sinks;
  auto stdout_sink = sinks::stdout_sink_st::instance();
  auto color_sink = make_shared<sinks::ansicolor_sink>(stdout_sink);
  sinks.push_back(color_sink);

  auto rotating_sink = make_shared<sinks::rotating_file_sink_st>
    (FILE_LOGGER_PATH, 1049000 * 10, 3);
  sinks.push_back(rotating_sink);
  
  auto combined_logger = make_shared<logger>("msns", sinks.begin(), sinks.end());
  register_logger(combined_logger);
}

int main(int argc, char* argv[]){
  chrono::high_resolution_clock::time_point start_t, end_t;
  start_t = chrono::high_resolution_clock::now();

  ////////////////////////////////////////////////////////////
  //                        OPTIONS                         //
  ////////////////////////////////////////////////////////////
  bool run = false;
  bool init = false;
  bool emailNtf = false;
  string reportLvl;
  string _initFolder;
  string initName;
  int initSize = 0;

  namespace po = boost::program_options;
  po::options_description gen_desc("Generic Options");
  gen_desc.add_options()
    ("help,h", "give the help list")
    ("version,v", "show version")
    ("run", "run folder analysis")
    ("init", "initialize folder creating config file");

  po::options_description run_desc("Run Options");
  run_desc.add_options()
    ("email,e", "enable email notification")
    ("report-lvl,L", po::value<string>(&reportLvl)->default_value("violation"),
     "set the report level. With `violation` only size violations are reported; "
     "with `all` all the monitored folders are added in the report");

  po::options_description init_desc("Init Options");

  init_desc.add_options()
    ("folder", po::value<string>(&_initFolder)->default_value("."),
     "folder to be inizialized")
    ("name", po::value<string>(&initName), "name for description purpose")
    ("size", po::value<int>(&initSize), "limit size in MiB");

  po::positional_options_description pos_desc;
  pos_desc.add("folder", 1);
  po::options_description cmdline_desc;
  cmdline_desc.add(gen_desc).add(run_desc).add(init_desc);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
	    .options(cmdline_desc).positional(pos_desc).run(), vm);
  po::notify(vm);

  if (vm.count("help")){
    cout << "Usage: msns [OPTIONS...]" << endl;
    cout << cmdline_desc << endl;
    return 0;
  }
  if (vm.count("version")){
    cout << VERSION_MSG << endl << endl;
    return 0;
  }

  ////////////////////////////////////////////////////////////
  //                        LOGGER                          //
  ////////////////////////////////////////////////////////////
  initLogger();
  auto logger = spdlog::get(MSNS_LOGGER);

  ////////////////////////////////////////////////////////////
  //                    OPTION VALIDATION                   //
  ////////////////////////////////////////////////////////////
  if (vm.count("run") && vm.count("init")){
    cerr << "Conflicting options: --run and --init can't be both active" << endl;
    return -1;
  }
  if (vm.count("run")){  // RUN
    run = true;
    if (reportLvl != "violation" && reportLvl != "all"){
      cerr << "Invalid report level" << endl;
      return -1;
    }

    emailNtf = vm.count("email") ? true : false;
  } else if (vm.count("init")) {  // INIT
    init = true;
    if (!validFolder(_initFolder)){
      cerr << "Invalid path" << endl;
      return -1;
    }

    if (!vm.count("name")){
      cerr << "Must specify name" << endl;
      return -1;
    }

    if (!vm.count("size")){
      cerr << "Must specify size limit" << endl;
      return -1;
    }
  } else {
    cerr << "Unspecified action" << endl;
    return -1;
  }

  ////////////////////////////////////////////////////////////
  //                        CONFING                         //
  ////////////////////////////////////////////////////////////
  logger->info("Global config path is {}", GLOBAL_CONFIG_PATH);
  GlobalConfig config(GLOBAL_CONFIG_PATH);
  config.load();

  ////////////////////////////////////////////////////////////
  //                        ROUTING                         //
  ////////////////////////////////////////////////////////////
  if (run){
    SPDLOG_DEBUG(logger, "Calling Run performer...");
    runAnalysis(config, reportLvl, emailNtf);
  } else if (init) {
    SPDLOG_DEBUG(logger, "Calling Init performer...");
    initFolder(_initFolder, initName, initSize);
  }

  end_t = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(end_t - start_t).count();
  logger->info("Finished in {} sec", duration / 1000.0);

  return 0;
}

void runAnalysis(const GlobalConfig& config, const string& reportLvl,
		 bool emailNtf){
  auto logger = spdlog::get(MSNS_LOGGER);
  ////////////////////////////////////////////////////////////
  //                       EXPLORING                        //
  ////////////////////////////////////////////////////////////
  logger->info("Found {} folders to explore", config.folders.size());
  vector<Report> reports;
  // Traveling
  for (auto folder : config.folders){
    unsigned long globSize;
    exploreDir(folder, globSize, reports);
  }

  ////////////////////////////////////////////////////////////
  //                      REPORTING                         //
  ////////////////////////////////////////////////////////////
  if (reportLvl == "violation"){
    // Filteringp
    SPDLOG_DEBUG(logger, "Report Lvl: {}. Filtering reports", reportLvl);
    SPDLOG_DEBUG(logger, "{} Reports before filtering", reports.size());
    remove_if(reports.begin(), reports.end(), [](Report r){ 
	return r.size < r.sizeLimit;
      });
  }
  logger->info("{} reports created", reports.size());

  EmailBuilder emailBuilder(config.machineName, config.emails, reportLvl);
  for (const Report& r : reports){
    logger->warn("Report: {}", r);
    if (emailNtf)
      emailBuilder.addReport(r);
  }

  if (!reports.empty() && emailNtf){
    CurlEmail emailHandler(config.emailUrl, config.emailUsername,
			  config.emailPassword, config.emailSsl);

    logger->info("Sending notification emails...");
    emailBuilder.sendAll(emailHandler, config.emailFrom);
  }
}

void initFolder(const string& initFolder, const string& initName, int initSize){
  string configFile = initFolder + PATH_SEPARATOR + LOCAL_CONFIG_NAME;
  if (fileExists(configFile)){
    cerr << "Folder already initialized" << endl;
    return;
  }

  LocalConfig locConfig;
  locConfig.name = initName;
  locConfig.sizeLimit = initSize;

  spdlog::get(MSNS_LOGGER)->info("Saving local config...");
  locConfig.save(configFile);
}


