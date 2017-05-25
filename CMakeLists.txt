cmake_minimum_required(VERSION 2.8.9)
set(PROJECT_NAME_STR msns)
set(PROJECT_VERSION "0.2.0")
project(${PROJECT_NAME_STR} C CXX)

add_compile_options("-std=c++11")


# ----------------
# Libraries
# ----------------
find_package(Boost 1.54 REQUIRED COMPONENTS program_options log)
find_package(Threads)
find_package(CURL REQUIRED)

link_directories(lib)


# ----------------
# Set Variables
# ----------------
set(COMMON_INCLUDES
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/lib
  ${Boost_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CURL_INCLUDE_DIRS})
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)


add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
# ----------------
# Sources
# ----------------
include_directories(${COMMON_INCLUDES})
file(GLOB SOURCES "src/*.cpp" "lib/**/*.cpp")


# ----------------
# Definitions
# ----------------
add_definitions(-DVERSION="0.2.0")

if(CMAKE_BUILD_TYPE MATCHES Debug)
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


# ----------------
# Compiling
# ----------------
set_property(SOURCE ${SOURCES}
  APPEND_STRING
  PROPERTY COMPILE_FLAGS "-Wall -pedantic -Wextra")

add_executable(${PROJECT_NAME_STR} ${SOURCES})
target_link_libraries(${PROJECT_NAME_STR}
  ${LIB_LOCATION}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${CURL_LIBRARIES})


# ----------------
# Install
# ----------------
if(NOT EXISTS "/etc/msns/config.ini")
  install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/
    DESTINATION "/etc/msns"
    DIRECTORY_PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_WRITE GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
endif()

if(NOT EXISTS "/var/log/msns")
  install(DIRECTORY
    DESTINATION "/var/log/msns"
    DIRECTORY_PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_WRITE GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
endif()

install(TARGETS msns
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

# ----------------
# Test
# ----------------
add_definitions(${MSVC_COMPILER_DEFS})
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES}
  include src)

list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES} ${SOURCES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

target_link_libraries(${PROJECT_TEST_NAME}
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a)
target_link_libraries(${PROJECT_TEST_NAME}
  ${LIB_LOCATION}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${CURL_LIBRARIES})

file(REMOVE ./test)
file(MAKE_DIRECTORY ./test/)
file(COPY ${PROJECT_SOURCE_DIR}/test/assets/ DESTINATION ./test/assets/)

add_test(test1 ${PROJECT_TEST_NAME})
