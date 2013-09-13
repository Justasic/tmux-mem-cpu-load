cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
    cmake_policy(VERSION 2.6)
endif(COMMAND cmake_policy)

project(tmux-mem-cpu-load CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(PLATFORM_SOURCE)

# Determine the platform and use it's source file for the compile
# OS X source
if(APPLE)
    set(PLATFORM_SOURCE Apple.cpp)
endif(APPLE)
# We're on FreeBSD
if (${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)
    set(PLATFORM_SOURCE FreeBSD.cpp)
endif (${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)
# We're on OpenBSD/NetBSD
if (${CMAKE_SYSTEM_NAME} STREQUAL OpenBSD OR ${CMAKE_SYSTEM_NAME} STREQUAL NetBSD)
    set(PLATFORM_SOURCE OpenBSD.cpp)
endif (${CMAKE_SYSTEM_NAME} STREQUAL OpenBSD OR ${CMAKE_SYSTEM_NAME} STREQUAL NetBSD)
# We're on linux
if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(PLATFORM_SOURCE Linux.cpp)
endif (${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)

add_executable(tmux-mem-cpu-load tmux-mem-cpu-load.cpp ${PLATFORM_SOURCE})
install(TARGETS tmux-mem-cpu-load
  RUNTIME DESTINATION bin
  )
