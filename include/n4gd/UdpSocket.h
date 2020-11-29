//Ѫ Ӕ Ѫ

#pragma once
#include "Ipv4.h"
#include "SocketInternal.h"

extern bool InitializeSockets();
extern void ShutdownSockets();


class UdpSocket {

public:
    UdpSocket() = default;
    ~UdpSocket();

    bool Open(unsigned short port);
    void Close();
    bool IsOpen() const;
    bool Send(const Ipv4& destination, const void* data, int size) const;
    int Receive(Ipv4& sender, void* data, int size) const;

private:

    UDP_SOCKET _socket{INVALID_SOCKET_UDP};
};
