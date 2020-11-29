//Ѫ Ӕ Ѫ

#ifndef N4GD_SOCKETINTERNAL_H
#define N4GD_SOCKETINTERNAL_H

#ifdef __linux__
    #include <netinet/in.h>
#elif _WIN32
    #include <WinSock2.h>
#endif

// Define platform constants
#ifdef __linux__
    #define INVALID_SOCKET_UDP -1
#elif _WIN32
    #define INVALID_SOCKET_UDP INVALID_SOCKET
#endif

// Define platform types
#ifdef __linux__
    typedef int UDP_SOCKET;
#elif _WIN32
    typedef SOCKET UDP_SOCKET;
    typedef int socklen_t;
#endif

#endif //N4GD_SOCKETINTERNAL_H
