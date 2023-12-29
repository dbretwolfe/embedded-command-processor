cmake_minimum_required(VERSION 3.1...3.27)

project(
  CmdProcCProj
  VERSION 1.0
  LANGUAGES C)

add_library(CmdProcC SHARED)
target_sources(CmdProcC PUBLIC command_functions.c command_processor.c)
set_target_properties(CmdProcC PROPERTIES LINKER_LANGUAGE C)