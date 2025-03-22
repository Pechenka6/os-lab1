cmake_minimum_required(VERSION 3.10)
project(EmployeeManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

add_library(employee employee.cpp) 

add_executable(main main.cpp)

target_link_libraries(main employee)

add_executable(creator creator.cpp)

target_link_libraries(creator employee)

add_executable(reporter reporter.cpp)

target_link_libraries(reporter employee)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_employee tests/test_employee.cpp)

target_link_libraries(test_employee ${GTEST_LIBRARIES} pthread employee)

add_test(NAME Test COMMAND test_employee)