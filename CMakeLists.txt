cmake_minimum_required(VERSION 2.8.9)
project(msns)

include(FindPkgConfig)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_compile_options("-std=c++11")

# Static lib
find_package(Boost 1.54 REQUIRED
  COMPONENTS program_options log)
find_package(Threads)
find_package(CURL REQUIRED)

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
