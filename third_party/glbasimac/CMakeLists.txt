cmake_minimum_required(VERSION 3.8)

add_library(glbasimac STATIC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE GLBASIMAC_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(glbasimac PRIVATE ${GLBASIMAC_SOURCES})
target_include_directories(glbasimac PUBLIC ../glbasimac/)
include_directories(glbasimac)

