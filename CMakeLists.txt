#
# Create a very simple ITk project
#

cmake_minimum_required (VERSION 2.6) 
set (Project_Name Palantir)
project (${Project_Name})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(BUILD_SHARED_LIBS OFF)

add_executable (${Project_Name} Palantir.cxx)
target_link_libraries(${Project_Name} ${ITK_LIBRARIES} ${VTK_LIBRARIES})