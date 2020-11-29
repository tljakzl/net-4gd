//Ѫ Ӕ Ѫ
#include <WinSock2.h>

bool UdpSocketUnlock(SOCKET& socket) {
    DWORD nonBlocking = 1;
    return ioctlsocket(socket, FIONBIO, &nonBlocking) == 0;
}
void UdpSocketClose(SOCKET& socket) {
    closesocket(socket);
}

void ShutdownSockets()
{
    WSACleanup();
}
bool InitializeSockets()
{
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
}
