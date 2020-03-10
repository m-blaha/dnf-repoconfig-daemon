cmake_minimum_required(VERSION 3.14)

project(daemontest CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(sdbus-c++ REQUIRED)

include(GNUInstallDirs)

pkg_check_modules(GLIB gio-unix-2.0>=2.46.0 REQUIRED)
include_directories(${GLIB_INCLUDE_DIRS})

add_definitions(-DLIBDNF_UNSTABLE_API)

include_directories(BEFORE SYSTEM /home/mblaha/src/libdnf)
include_directories(BEFORE SYSTEM /home/mblaha/src/libdnf/libdnf)

SET(DAEMON_SRCS
    main.cpp
    Context.cpp
)

add_executable(daemontest ${DAEMON_SRCS})

target_link_libraries(daemontest
                      dnf
                      rpm
                      sdbus-c++
                      ${CMAKE_DL_LIBS}
                      ${GLIB_LIBRARIES}
                      ${GLIB_GOBJECT_LIBRARIES}
                      ${GLIB_GIO_LIBRARIES}
                      ${GLIB_GIO_UNIX_LIBRARIES}
                      ${RPM_LIBRARIES}
                      )

install(TARGETS daemontest RUNTIME DESTINATION bin)
