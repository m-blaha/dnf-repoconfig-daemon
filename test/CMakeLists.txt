cmake_minimum_required(VERSION 3.14)

project(daemontest CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(sdbus-c++ REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET gio-unix-2.0>=2.46.0)
pkg_check_modules(LIBDNF REQUIRED IMPORTED_TARGET libdnf)
# XXX libdnf/repo/Repo.hpp is not public
include_directories(BEFORE SYSTEM /home/mblaha/src/libdnf)
include_directories(BEFORE SYSTEM /home/mblaha/src/libdnf/libdnf)

add_definitions(-DLIBDNF_UNSTABLE_API)

SET(DAEMON_SRCS
    main.cpp
    Context.cpp
    Repository.cpp
)

add_executable(daemontest ${DAEMON_SRCS})

target_link_libraries(
    daemontest
    PRIVATE
        sdbus-c++
        PkgConfig::GLIB
        PkgConfig::LIBDNF
)

install(TARGETS daemontest RUNTIME DESTINATION bin)
