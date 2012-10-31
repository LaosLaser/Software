###
# Artron B.V
# MultiLiwa Project
###
cmake_minimum_required(VERSION 2.8)

project(cups-laos CXX)

set(PROJECT_VERSION "0.1")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Set the source files
set(SOURCE
Config.cpp
LogEvent.cpp
main.cpp
	)

add_executable( cups-laos ${SOURCE} )

set(CMAKE_CXX_FLAGS "-Wall")

include_directories(
    ${SOURCE_DIR}
    ${BUILD_DIR}
)


###
# Source Packaging
###
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MultiLiWa Project")
set(CPACK_PACKAGE_VENDOR "Artron B.V")
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
#set(CPACK_PACKAGE_EXECUTABLES "sdl" "MultiLiWa Simulator")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${ARCHIVE_NAME}")

include(CPack)
