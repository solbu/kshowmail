project(kshowmail)

# version number
set (KSHOWMAIL_VERSION 4.0)

#configure a header file to pass some of the CMake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/buildConfig.h.in"
  "${PROJECT_SOURCE_DIR}/src/buildConfig.h"
)

find_package(KDE4 REQUIRED)
find_package(KdepimLibs REQUIRED)

include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( pics )
add_subdirectory(sounds)
add_subdirectory(po)