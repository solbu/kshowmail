project(kshowmail)

cmake_minimum_required(VERSION 2.6)

# version number
set (KSHOWMAIL_VERSION 5.0)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(ECMInstallIcons)

#INCLUDE(MacroLibrary)
INCLUDE(FindPackageHandleStandardArgs)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS DocTools CoreAddons Solid)
find_package(KF5Libkdepim REQUIRED)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

#configure a header file to pass some of the CMake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/buildConfig.h.in"
  "${PROJECT_SOURCE_DIR}/src/buildConfig.h"
)


include_directories(
  ${QT_INCLUDES}
)


add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( pics )
add_subdirectory(sounds)
add_subdirectory(po)
add_subdirectory(man)
