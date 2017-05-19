cmake_minimum_required(VERSION 2.8.9)
project(msns)

include(FindPkgConfig)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_compile_options("-std=c++11")

# Custom constant
add_definitions(-DVERSION="0.1.0")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(GLOB_CONFIG_PATH "./config.ini")
  set(FILE_LOGGER_PATH "./msns.log")
  add_definitions(-DSPDLOG_DEBUG_ON)
  add_definitions(-DSPDLOG_TRACE_ON)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -O3")
  set(GLOB_CONFIG_PATH "/etc/msns/config.ini")
  set(FILE_LOGGER_PATH "/var/log/msns/msns.log")
endif()

message(STATUS "Global config path: " ${GLOB_CONFIG_PATH})
message(STATUS "File logger path: " ${FILE_LOGGER_PATH})

add_definitions(
  -DGLOBAL_CONFIG_PATH="${GLOB_CONFIG_PATH}"
  -DFILE_LOGGER_PATH="${FILE_LOGGER_PATH}")

# Static lib
find_package(Boost 1.54 REQUIRED
  COMPONENTS program_options log)
find_package(Threads)
find_package(CURL REQUIRED)

# FILES
include_directories(include src
  ${Boost_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CURL_INCLUDE_DIRS})
file(GLOB SOURCES "src/*.cpp")

set_property(SOURCE ${SOURCES}
  APPEND_STRING
  PROPERTY COMPILE_FLAGS "-Wall -pedantic -Wextra")
add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(msns ${SOURCES})
target_link_libraries(msns
  ${LIB_LOCATION}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${CURL_LIBRARIES})
target_compile_options(msns PUBLIC ${VMIME_CFLAGS_OTHER})


# INSTALL
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/
  DESTINATION "/etc/msns"
  DIRECTORY_PERMISSIONS
  OWNER_WRITE OWNER_READ OWNER_EXECUTE
  GROUP_WRITE GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE)
install(DIRECTORY
  DESTINATION "/var/log/msns"
  DIRECTORY_PERMISSIONS
  OWNER_WRITE OWNER_READ OWNER_EXECUTE
  GROUP_WRITE GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
install(TARGETS msns
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
