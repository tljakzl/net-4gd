//Ѫ Ӕ Ѫ

#include <n4gd/Connection.h>
#include <iostream>
#include <n4gd/Utils.h>

#define LOG_SERVER(x) std::cout <<"[SERVER] "<< x

const int ServerPort = 7777;
const int ClientPort = 30001;
const int ProtocolId = 0x99887766;
const float DeltaTime = 0.25f;
const float TimeOut = 10.0f;


int main(int argc, char* argv[])
{
    if (!InitializeSockets()) {
        LOG_SERVER("Failed to initialize sockets\n");
        return 1;
    }

    n4gd::Connection connection(ProtocolId, TimeOut);

    if (!connection.Start(ServerPort)) {
        LOG_SERVER("Could not start connection on port " << ServerPort << "\n");
        return 1;
    }

    connection.Listen();

    while (true) {
        if (connection.IsConnected()) {
            unsigned char packet[] = "server to client";
            connection.SendPacket(packet, sizeof(packet));
        }

        while (true) {
            unsigned char packet[256];
            int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
            if (bytes_read == 0)
                break;
            LOG_SERVER("Received packet from client\n");
        }

        connection.Update(DeltaTime);
        Wait(DeltaTime);
    }

    ShutdownSockets();
    return 0;
}
