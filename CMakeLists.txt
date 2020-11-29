cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
project(n4gd)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(N4GD_DIR ${INCLUDE_DIR}/n4gd)
set(CMAKE_CXX_STANDARD 17)

project(n4gd VERSION 0.1 DESCRIPTION "lib for net interaction")

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_library(n4gd
        ${SRC_DIR}/Address/Ipv4.cpp
        ${SRC_DIR}/Socket/UdpSocket.cpp
        ${SRC_DIR}/pch.cpp
        ${N4GD_DIR}/Ipv4.h
        ${N4GD_DIR}/UdpSocket.h
        ${N4GD_DIR}/pch.h include/n4gd/SocketInternal.h)

if(WIN32)
    target_link_libraries(n4gd wsock32 ws2_32)
    target_sources(n4gd PRIVATE ${SRC_DIR}/Socket/Platform/UdpSocket_WIN32.cpp)
elseif(APPLE)
    target_sources(n4gd PRIVATE ${SRC_DIR}/Socket/Platform/UdpSocket_APPLE.cpp)
elseif(UNIX AND NOT APPLE)
    target_sources(n4gd PRIVATE ${SRC_DIR}/Socket/Platform/UdpSocket_UNIX.cpp)
endif()


target_include_directories(n4gd PRIVATE ${INCLUDE_DIR})
set_target_properties(n4gd PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(n4gd PROPERTIES SOVERSION 1)
#set_target_properties(n4gd PROPERTIES PUBLIC_HEADER ${N4GD_DIR}}/n4gd.h)

install(TARGETS n4gd
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
configure_file(n4gd.pc.in n4gd.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/n4gd.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
