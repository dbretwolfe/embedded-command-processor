cmake_minimum_required(VERSION 3.1...3.27)

project(
  CmdProcCppTestProj
  VERSION 1.0
  LANGUAGES C CXX)

add_executable(CmdProcCppTest)
target_sources(CmdProcCppTest PUBLIC 
  test.cpp
  command_functions.cpp
)
target_include_directories(CmdProcCppTest PUBLIC ./ ../../command_processor_cpp/)
target_link_libraries(CmdProcCppTest PUBLIC CmdProcCpp PRIVATE GTest::gtest_main)
target_compile_features(CmdProcCppTest PUBLIC cxx_std_23)

include(GoogleTest)
gtest_discover_tests(CmdProcCppTest)
