cmake_minimum_required(VERSION 3.17.2)

project(election LANGUAGES C )

set(MTM_FLAGS "-std=c99  -Wall -pedantic-errors -Werror -g")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS})

link_directories(.)
add_executable(election election.c area.c tribe.c tests/electionTests.c tests/electionTestsMain.c)
target_link_libraries(election libmap.a)

#add_executable(election election.c area.c tribe.c mtm_map/map.c tests/electionTestsExample.c)
