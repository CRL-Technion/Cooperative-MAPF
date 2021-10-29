project(cbs)

cmake_minimum_required (VERSION 2.6)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories("get_best_assignments")
add_subdirectory(get_best_assignments)
include_directories("get_best_assignments")
file(GLOB SOURCES_TA "get_best_assignments/*.cpp")

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")
add_executable(cocbs ${SOURCES} ${SOURCES_TA})

# Find Boost
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)
include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries(cocbs ${Boost_LIBRARIES})

# Link get_best_assignments
target_link_libraries(cocbs task_assignment)

