cmake_minimum_required (VERSION 2.8)

# cmake versions:
# Debian 6 - 2.8.2
# Debian 7 - 2.8.9
# CentOS 6 - 2.8.12

project(FastNetMon)

set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 1)

# Set path to home compiled PF_RING
set(PFRING_INCLUDE_DIRS /opt/pf_ring/include)
set(PFRING_LIBRARIES /opt/pf_ring/lib/libpfring.so)

# Some defines for FastNetMon compiation tuning
add_definitions(-DENABLE_CONNTRACKING)
add_definitions(-DPF_RING)

# Our LPM library
add_library(libpatricia STATIC libpatricia/patricia.c)

# Main tool
add_executable(fastnetmon fastnetmon.cpp)

# Find boost: http://www.cmake.org/cmake/help/v3.0/module/FindBoost.html
# Detailed errors
set(Boost_DETAILED_FAILURE_MSG ON)
find_package(Boost COMPONENTS thread regex REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(fastnetmon ${Boost_LIBRARIES})
endif()

# Try to find ncurses library
find_package(Curses REQUIRED)

if(CURSES_FOUND) 
    include_directories(${CURSES_INCLUDE_DIRS})
    target_link_libraries(fastnetmon ${CURSES_LIBRARIES})
endif()

# External libs
target_link_libraries(fastnetmon log4cpp)
target_link_libraries(fastnetmon numa)
target_link_libraries(fastnetmon pthread)

# Our libs
target_link_libraries(fastnetmon libpatricia)

# Custom PF_RING
include_directories(${PFRING_INCLUDE_DIRS})
target_link_libraries(fastnetmon ${PFRING_LIBRARIES})