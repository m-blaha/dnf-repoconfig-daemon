cmake_minimum_required(VERSION 3.14)

project(daemontest CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SYSCONFDIR /etc)

find_package(PkgConfig REQUIRED)
find_package(sdbus-c++ REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET gio-unix-2.0>=2.46.0)
pkg_check_modules(LIBDNF REQUIRED IMPORTED_TARGET libdnf)

add_definitions(-DLIBDNF_UNSTABLE_API)

SET(DAEMON_SRCS
    main.cpp
)

add_subdirectory(repoconf)
add_subdirectory(dbus)

add_executable(dnf-repoconfig-daemon ${DAEMON_SRCS})

target_link_libraries(
    dnf-repoconfig-daemon
    PRIVATE
        sdbus-c++
        PkgConfig::GLIB
        PkgConfig::LIBDNF
)

install(TARGETS dnf-repoconfig-daemon RUNTIME DESTINATION bin)
