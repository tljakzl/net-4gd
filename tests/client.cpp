//Ѫ Ӕ Ѫ

#include <n4gd/Connection.h>
#include <iostream>
#include <n4gd/Utils.h>

const int ServerPort = 30000;
const int ClientPort = 30001;
const int ProtocolId = 0x99887766;
const float DeltaTime = 0.25f;
const float SendRate = 0.25f;
const float TimeOut = 10.0f;

#define LOG_CLIENT(x) std::cout <<"[CLIENT] "<< x

int main(int argc, char* argv[])
{
    if (!InitializeSockets()) {
        LOG_CLIENT("Failed to initialize sockets\n");
        return 1;
    }

    n4gd::Connection connection(ProtocolId, TimeOut);

    bool connected = false;
    connection.Start(ClientPort);
    connection.Connect(Ipv4(127, 0 , 0, 1, ServerPort));
    while (true)
    {
        if (!connected && connection.IsConnected())
        {
            LOG_CLIENT("Client connected to server\n");
            connected = true;
        }

        if (!connected && connection.ConnectFailed())
        {
            LOG_CLIENT("Connection failed\n");
            break;
        }

        unsigned char packet[] = "client to server";
        connection.SendPacket(packet, sizeof(packet));

        while (true)
        {
            unsigned char packet[256];
            int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
            if (bytes_read == 0)
                break;
            LOG_CLIENT("Received packet from server\n");
        }

        connection.Update(DeltaTime);
        Wait(DeltaTime);
    }

    ShutdownSockets();

    return 0;
}

