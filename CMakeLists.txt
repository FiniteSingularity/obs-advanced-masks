# --- Detect if the plugin is build out of tree or not ---
if(CMAKE_PROJECT_NAME STREQUAL "obs-studio")
  set(BUILD_OUT_OF_TREE OFF)
#   if(OBS_CMAKE_VERSION VERSION_GREATER_EQUAL 3.0.0)
#     legacy_check()
#   endif()
else()
  set(BUILD_OUT_OF_TREE ON)
  cmake_minimum_required(VERSION 3.18)
endif()

project(obs-advanced-masks VERSION 1.5.3)
set(PROJECT_FULL_NAME "Advanced Masks")

# Set new UUIDs when you start to create a new plugin.
set(MACOS_PACKAGE_UUID "7789455d-9778-42bf-9a4e-40f2a6441e45")
set(MACOS_INSTALLER_UUID "84aa3f86-fa80-481e-87c7-48c053f8fc75")

add_library(${PROJECT_NAME} MODULE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

target_sources(${PROJECT_NAME} PRIVATE
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
	src/version.h)

if(BUILD_OUT_OF_TREE)
	find_package(libobs REQUIRED)
	find_package(obs-frontend-api REQUIRED)
	include(cmake/ObsPluginHelpers.cmake)
	find_qt(COMPONENTS Widgets Svg Xml COMPONENTS_LINUX Gui)
	set(OBS_FRONTEND_API_NAME "obs-frontend-api")

	include(FetchContent)
	FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
	FetchContent_MakeAvailable(json)
else()
	if(OBS_VERSION VERSION_GREATER_EQUAL 30.1.0)
		find_package(Qt6 COMPONENTS Core Widgets Svg Xml)
		# find_package(nlohmann_json REQUIRED)
		include(FetchContent)
		FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
		FetchContent_MakeAvailable(nlohmann_json)
	else()
		find_qt(COMPONENTS Widgets Svg Xml COMPONENTS_LINUX Gui)
		if(OS_WINDOWS)
			include(FetchContent)
			FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
			FetchContent_MakeAvailable(nlohmann_json)
		endif()
	endif()
	set(OBS_FRONTEND_API_NAME "frontend-api")
endif()

if(OS_WINDOWS)
	get_filename_component(ISS_FILES_DIR "${CMAKE_BINARY_DIR}\\..\\package" ABSOLUTE)
	file(TO_NATIVE_PATH "${ISS_FILES_DIR}" ISS_FILES_DIR)
	get_filename_component(ISS_PACKAGE_DIR "${CMAKE_PACKAGE_PREFIX}\\.." ABSOLUTE)
	file(TO_NATIVE_PATH "${ISS_PACKAGE_DIR}" ISS_PACKAGE_DIR)
	get_filename_component(ISS_SOURCE_DIR "${PROJECT_SOURCE_DIR}" ABSOLUTE)
	file(TO_NATIVE_PATH "${ISS_SOURCE_DIR}" ISS_SOURCE_DIR)
	configure_file("installer.iss.in"
			"${PROJECT_BINARY_DIR}/installer.iss"
		)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc)
	target_sources(${PROJECT_NAME} PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc)
elseif(OS_MACOS)
	set(MACOS_BUNDLEID "com.finitesingularity.${PROJECT_NAME}")
	set(MACOSX_PLUGIN_GUI_IDENTIFIER "${MACOS_BUNDLEID}")
	set(MACOSX_PLUGIN_BUNDLE_VERSION "${PROJECT_VERSION}")
	set(MACOSX_PLUGIN_SHORT_VERSION_STRING "1")
	configure_file(cmake/bundle/macos/installer-macos.pkgproj.in ${CMAKE_BINARY_DIR}/installer-macos.generated.pkgproj)
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

find_package(CURL REQUIRED)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)

if(OS_MACOS)
	target_link_libraries(${PROJECT_NAME}
		Qt::Widgets
		Qt::Svg
		Qt::Xml
		nlohmann_json
		CURL::libcurl
		OBS::libobs
		OBS::${OBS_FRONTEND_API_NAME}
		"-framework Security")
else()
	target_link_libraries(${PROJECT_NAME}
		Qt::Widgets
		Qt::Svg
		Qt::Xml
		nlohmann_json
		CURL::libcurl
		OBS::libobs
		OBS::${OBS_FRONTEND_API_NAME})
endif()

if(BUILD_OUT_OF_TREE)
    if(NOT LIB_OUT_DIR)
        set(LIB_OUT_DIR "/lib/obs-plugins")
    endif()
    if(NOT DATA_OUT_DIR)
        set(DATA_OUT_DIR "/share/obs/obs-plugins/${PROJECT_NAME}")
    endif()
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIB_OUT_DIR})
    install(DIRECTORY data/locale data/shaders
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${DATA_OUT_DIR})
    setup_plugin_target(${PROJECT_NAME})
else()
    if(OBS_CMAKE_VERSION VERSION_GREATER_EQUAL 3.0.0)
        set_target_properties_obs(${PROJECT_NAME} PROPERTIES FOLDER "plugins/finitesingularity" PREFIX "")
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "plugins/finitesingularity")
        setup_plugin_target(${PROJECT_NAME})
    endif()
endif()

