cmake_minimum_required(VERSION 3.1...3.27)

project(
  CmdProcCppProj
  VERSION 1.0
  LANGUAGES CXX)

add_library(CmdProcCpp SHARED)
target_sources(CmdProcCpp PUBLIC command.cpp command_processor.cpp)
set_target_properties(CmdProcCpp PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(CmdProcCpp PUBLIC cxx_std_23)