cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(compilerconfig)
include(defaults)
include(helpers)

# Path were nlohmann_json is supposed to be installed inside the Flatpak runtime
set(JSON_TARBALL_PATH "${CMAKE_SOURCE_DIR}/json.tar.xz")

add_library(${CMAKE_PROJECT_NAME} MODULE)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

# Check if nlohmann_json exists
if(EXISTS "${JSON_TARBALL_PATH}")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xJf "${JSON_TARBALL_PATH}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )
  add_library(nlohmann_json INTERFACE)
  target_include_directories(nlohmann_json INTERFACE "${CMAKE_BINARY_DIR}/json/single_include")
else()
  include(FetchContent)
  FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
  FetchContent_MakeAvailable(json)
endif()

find_package(CURL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json CURL::libcurl)

if(APPLE)
  target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PRIVATE "$<LINK_LIBRARY:FRAMEWORK,Security.framework>" "$<LINK_LIBRARY:FRAMEWORK,CoreFoundation.framework>"
  )
endif()

if(ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets Core Svg Xml)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg Qt6::Xml)
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header -Wno-comma>
  )
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON
  )
endif()

target_sources(
  ${PROJECT_NAME}
  PRIVATE
    src/advanced-masks-filter.c
    src/base-filter.h
    src/color-adjustments.c
    src/color-adjustments.h
    src/obs-advanced-masks-plugin.c
    src/advanced-masks-filter.h
    src/advanced-masks.h
    src/mask-bsm.c
    src/mask-bsm.h
    src/mask-chroma-key.c
    src/mask-chroma-key.h
    src/mask-feather.c
    src/mask-feather.h
    src/mask-font-awesome.cpp
    src/mask-font-awesome.hpp
    src/mask-gradient.c
    src/mask-gradient.h
    src/mask-shape.c
    src/mask-shape.h
    src/mask-source.c
    src/mask-source.h
    src/mask-svg.c
    src/mask-svg.h
    src/obs-utils.c
    src/obs-utils.h
    src/svg-utils.cpp
    src/svg-utils.hpp
    src/utils.c
    src/utils.h
    src/version.h
)

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})
