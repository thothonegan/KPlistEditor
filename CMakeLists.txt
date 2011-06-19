cmake_minimum_required(VERSION 2.6)

project(kplisteditor)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

#add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
