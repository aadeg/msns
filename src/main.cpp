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
#include <iomanip>

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include "const.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "travel_files.hpp"
#include "email.hpp"
#include "report.hpp"

using namespace std;
using namespace msns;

void runAnalysis(const GlobalConfig& config, const string& reportLvl, bool emailNtf);
void initFolder(const string& initFolder, const string& initName, int initSize);


void init_log() {
  namespace logging = boost::log;
  logging::core::get()->set_filter
    (logging::trivial::severity >= logging::trivial::debug);
}

int main(int argc, char* argv[]){
  init_log();

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

  // Option validation
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
  GlobalConfig config(GLOBAL_CONFIG_PATH);
  config.load();

  ////////////////////////////////////////////////////////////
  //                        ROUTING                         //
  ////////////////////////////////////////////////////////////
  if (run){
    runAnalysis(config, reportLvl, emailNtf);
  } else if (init) {
    initFolder(_initFolder, initName, initSize);
  }

  return 0;
}

void runAnalysis(const GlobalConfig& config, const string& reportLvl,
		 bool emailNtf){
  ////////////////////////////////////////////////////////////
  //                       TRAVELING                        //
  ////////////////////////////////////////////////////////////
  list<Report> reports;
  // Traveling
  for (auto folder : config.folders){
    unsigned long globSize;
    travelDir(folder, globSize, reports);
  }

  ////////////////////////////////////////////////////////////
  //                      REPORTING                         //
  ////////////////////////////////////////////////////////////
  BOOST_LOG_TRIVIAL(info) << reports.size() << " reports created";
  if (reportLvl == "violation"){
    // Filtering
    reports.remove_if([](Report r){
	return r.size < r.sizeLimit;
      });
    BOOST_LOG_TRIVIAL(info) << reports.size() << " reports left after filtering";
  }

  EmailBuilder emailBuilder(config.machineName, config.emails, reportLvl);
  for (const Report& r : reports){
    double perc = int(r.size - r.sizeLimit) / double(r.sizeLimit) * 100;
    BOOST_LOG_TRIVIAL(warning) << "## REPORT ##" << endl
			       << "Name: " << r.name << endl
			       << "Path: " << r.path << endl
			       << "Size: " << r.size << endl
			       << "SLmt: " << r.sizeLimit << endl
			       << "Perc: " << setprecision(3)
			       << perc << "%";
    if (emailNtf)
      emailBuilder.addReport(r);
  }

  if (!reports.empty() && emailNtf){
    CurlEmail emailHandler(config.emailUrl, config.emailUsername,
			  config.emailPassword, config.emailSsl);

    emailBuilder.debug();
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

  locConfig.save(configFile);
}


