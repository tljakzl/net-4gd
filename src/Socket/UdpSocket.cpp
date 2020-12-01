//Ѫ Ӕ Ѫ

#include <n4gd/pch.h>
#include <n4gd/UdpSocket.h>

extern bool UdpSocketUnlock(UDP_SOCKET& socket);
extern void UdpSocketClose(UDP_SOCKET& socket);

bool UdpSocket::Open(Ipv4::PORT port)
{
    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_socket == INVALID_SOCKET_UDP) {
        std::cerr << "Failed to create UDP socket!\n";
        return false;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(_socket, reinterpret_cast<const sockaddr*>(&address), sizeof(sockaddr_in)) < 0) {
        std::cerr << "Failed to bind UDP socket!\n";
        Close();
        return false;
    }

    if (!UdpSocketUnlock(_socket)) {
        std::cerr << "Failed to set non-blocking socket!\n";
        Close();
        return false;
    }
    _port = port;

    return true;
}

void UdpSocket::Close()
{
    _port = 0;
    if (_socket != INVALID_SOCKET_UDP) {
        UdpSocketClose(_socket);
        _socket = INVALID_SOCKET_UDP;
    }
}

bool UdpSocket::Send(const Ipv4& destination, const void* data, int size) const
{
    sockaddr_in sockAddrIn{};
    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_addr.s_addr = htonl(destination.GetAddress());
    sockAddrIn.sin_port = htons(destination.GetPort());

    int sentBytes = sendto(_socket, reinterpret_cast<const char*>(data), size, 0, reinterpret_cast<sockaddr*>(&sockAddrIn), sizeof(sockaddr_in));
    if (sentBytes != size) {
        std::cerr << "Failed to send packet: return value = " <<  sentBytes << std::endl;
        return false;
    }
    return true;
}

int UdpSocket::Receive(Ipv4& sender, void* data, int size) const
{
    if (!IsOpen()) {
        return 0;
    }

    sockaddr_in from{};
    socklen_t fromSize = sizeof(from);
    int receivedBytes = recvfrom(_socket, reinterpret_cast<char*>(data), size, 0, reinterpret_cast<sockaddr*>(&from), &fromSize);

    if (receivedBytes <= 0) {
        return 0;
    }

    unsigned int protocol = ntohl(from.sin_addr.s_addr);
    unsigned int port = ntohs(from.sin_port);
    sender = Ipv4(protocol, port);
    return receivedBytes;
}

bool UdpSocket::IsOpen() const
{
    return _socket != INVALID_SOCKET_UDP;
}

UdpSocket::~UdpSocket()
{
    Close();
}

Ipv4::PORT UdpSocket::GetPort() const
{
    return _port;
}
