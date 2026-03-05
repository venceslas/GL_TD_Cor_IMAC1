cmake_minimum_required(VERSION 3.10)

add_library(glbasimac STATIC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE GLBASIMAC_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(glbasimac PRIVATE ${GLBASIMAC_SOURCES})

if (MSVC)
target_compile_options(glbasimac PRIVATE /W3)
else()
target_compile_options(glbasimac PRIVATE -std=c++17 -Wall -Wextra)
endif()

target_include_directories(glbasimac PUBLIC ../glbasimac/)
# --- Includes (ton layout : glbasimac/ et tools/) ---
target_include_directories(glbasimac
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/glbasimac"
    "${CMAKE_CURRENT_SOURCE_DIR}/tools"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

# --- Assets dir (vient du projet racine) ---
# fallback si quelqu’un compile la lib seule
set(GLBASIMAC_ASSETS_DIR_DEFAULT "${CMAKE_SOURCE_DIR}/assets")

# si le root a défini TDFRAMEWORK_ASSETS_DIR, on l'utilise
if (DEFINED TDFRAMEWORK_ASSETS_DIR)
  set(GLBASIMAC_ASSETS_DIR "${TDFRAMEWORK_ASSETS_DIR}")
else()
  set(GLBASIMAC_ASSETS_DIR "${GLBASIMAC_ASSETS_DIR_DEFAULT}")
endif()

# --- Header généré ---
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/glbasimac_config.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/generated/GlbaSimacConfig.hpp"
  @ONLY
)
include_directories(glbasimac)

