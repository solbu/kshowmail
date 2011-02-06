project(kshowmail)

cmake_minimum_required(VERSION 2.6)

# version number
set (KSHOWMAIL_VERSION 4.1)

find_package(KDE4 REQUIRED)

include (KDE4Defaults)
INCLUDE(MacroLibrary)
INCLUDE(FindPackageHandleStandardArgs)

find_package(KdepimLibs REQUIRED)

#configure a header file to pass some of the CMake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/buildConfig.h.in"
  "${PROJECT_SOURCE_DIR}/src/buildConfig.h"
)


include_directories(
  ${KDE4_INCLUDES}
  ${QT_INCLUDES}
)


add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( pics )
add_subdirectory(sounds)
add_subdirectory(po)