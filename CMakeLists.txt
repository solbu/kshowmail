project(kshowmail)


find_package(KDE4 REQUIRED)
find_package(KdepimLibs REQUIRED)

include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( pics )
add_subdirectory(sounds)