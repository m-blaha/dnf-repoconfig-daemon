cmake_minimum_required(VERSION 3.14)

project(dnf-repoconfig-daemon CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SYSCONFDIR /etc)

find_package(PkgConfig REQUIRED)
find_package(sdbus-c++ REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET gio-unix-2.0>=2.46.0)
pkg_check_modules(LIBDNF REQUIRED IMPORTED_TARGET libdnf)

add_definitions(-DLIBDNF_UNSTABLE_API)

add_subdirectory(dbus)
add_subdirectory(polkit)
add_subdirectory(basic-api)

install(TARGETS ${CLIENT_BIN} RUNTIME DESTINATION bin)
