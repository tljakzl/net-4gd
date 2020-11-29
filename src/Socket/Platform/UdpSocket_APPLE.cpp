//Ѫ Ӕ Ѫ

bool UdpSocketUnlock(SOCKET& socket)
{
    int nonBlocking = 1;
    return fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) != -1;
}

void UdpSocketClose(SOCKET& socket){
    close(socket);
}

void ShutdownSockets()
{

}
bool InitializeSockets()
{
    return true;
}