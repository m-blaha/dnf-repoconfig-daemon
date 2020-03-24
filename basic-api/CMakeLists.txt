cmake_minimum_required(VERSION 3.14)

project(dnf-repoconf-daemon CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SYSCONFDIR /etc)

find_package(PkgConfig REQUIRED)
find_package(sdbus-c++ REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET gio-unix-2.0>=2.46.0)
pkg_check_modules(LIBDNF REQUIRED IMPORTED_TARGET libdnf)

add_definitions(-DLIBDNF_UNSTABLE_API)

SET(SERVER_BIN ${PROJECT_NAME}-server)
SET(SERVER_SRCS
    main.cpp
    repoconf.cpp
    configuration.cpp
)
add_executable(${SERVER_BIN} ${SERVER_SRCS})
target_link_libraries(
    ${SERVER_BIN}
    PRIVATE
        sdbus-c++
        PkgConfig::GLIB
        PkgConfig::LIBDNF
)
install(TARGETS ${SERVER_BIN} RUNTIME DESTINATION bin)



SET(CLIENT_BIN ${PROJECT_NAME}-client)
SET(CLIENT_SRCS
    client.cpp
)
add_executable(${CLIENT_BIN} ${CLIENT_SRCS})
target_link_libraries(
    ${CLIENT_BIN}
    PRIVATE
        sdbus-c++
)
install(TARGETS ${CLIENT_BIN} RUNTIME DESTINATION bin)
