add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest)

add_executable(testMain main.cpp)
target_include_directories(testMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest/googletest/include)
target_link_libraries(testMain n4gd)
target_link_libraries(testMain gtest)
install(TARGETS testMain
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(client client.cpp)
target_link_libraries(client n4gd)
install(TARGETS client
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(server server.cpp)
target_link_libraries(server n4gd)
install(TARGETS server
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

