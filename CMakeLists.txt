cmake_minimum_required(VERSION 3.12)
project(NativeDecrypt)

set(CMAKE_CXX_STANDARD 14)

set(JAVA_HOME $ENV{JAVA_HOME})

set(INC_DIR ${JAVA_HOME}/include ${JAVA_HOME}/include/win32 include/pugixml)
set(LINK_DIR ${JAVA_HOME}/lib libs/${CMAKE_BUILD_TYPE} )
set(SOURCE_FILES decrypt.cpp ParamConfig.hpp ParseEncConfig.hpp ParseEncConfig.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../libs/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../libs/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/${CMAKE_BUILD_TYPE})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../ModifyJar/Decrypt")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(jvm libpugixml)

add_library(NativeDecrypt SHARED ${SOURCE_FILES})