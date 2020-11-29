//Ѫ Ӕ Ѫ

#include <n4gd/SocketInternal.h>
#include <fcntl.h>
#include <unistd.h>

bool UdpSocketUnlock(UDP_SOCKET& socket)
{
    int nonBlocking = 1;
    return fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) != -1;
}

void UdpSocketClose(UDP_SOCKET& socket)
{
    close(socket);
}

void ShutdownSockets(){}

bool InitializeSockets()
{
    return true;
}
