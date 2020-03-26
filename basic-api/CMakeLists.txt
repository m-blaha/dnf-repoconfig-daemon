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
