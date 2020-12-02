//Ѫ Ӕ Ѫ

#include <n4gd/Connection.h>
#include <n4gd/Utils.h>
#include <gtest/gtest.h>

typedef unsigned char PACKET;
typedef std::basic_string<PACKET, std::char_traits<PACKET>, std::allocator<PACKET>> PacketString;

inline void SendPacket(n4gd::Connection& connection, const PacketString& packet);
inline void ReceivePacket(n4gd::Connection& connection);
inline void ReceivePacket(n4gd::Connection& connection, const PACKET expected[]);

const int serverPort = 30000;
const int clientPort = 30001;
const int protocolId = 0x11112222;
const float deltaTime = 0.001f;
const float timeOut = 0.1f;

const Ipv4 addrDefault(127, 0, 0, 1, serverPort);

const PACKET c2s[] = "client to server";
const PACKET s2c[] = "server to client";

#define DEFAULT_TEST()                                          \
client.Connect(addrDefault);                                    \
while(true){                                                    \
if (client.IsConnected() && server.IsConnected()) {break;}      \
if (client.ConnectFailed() || server.ConnectFailed()) {break;}  \
SendPacket(client, c2s);                                        \
SendPacket(server, s2c);                                        \
ReceivePacket(client,s2c);                                      \
ReceivePacket(server,c2s);                                      \
client.Update(deltaTime);                                       \
server.Update(deltaTime);                                       \
Wait(deltaTime);                                                \
}                                                               \
EXPECT_TRUE(server.IsConnected());                              \
EXPECT_TRUE(client.IsConnected())                               \


#define JOIN_DEFAULT()                            \
n4gd::Connection client(protocolId, timeOut);     \
n4gd::Connection server(protocolId, timeOut);     \
EXPECT_TRUE(client.Start(clientPort));            \
EXPECT_TRUE(server.Start(serverPort));            \
server.Listen();                                  \
DEFAULT_TEST()                                    \


TEST(TestName, Join_Test)
{
    InitializeSockets();
    JOIN_DEFAULT();
    ShutdownSockets();
}

TEST(TestName, Join_Time_Out)
{
    InitializeSockets();
    n4gd::Connection client(protocolId, timeOut);
    EXPECT_TRUE(client.Start(clientPort));
    client.Connect(addrDefault);
    while(true)
    {
        if (!client.IsConnecting()) {
            break;
        }
        SendPacket(client, c2s);
        ReceivePacket(client);
        client.Update(deltaTime);
        Wait(deltaTime);
    }
    EXPECT_FALSE(client.IsConnected());
    EXPECT_TRUE(client.ConnectFailed());
    ShutdownSockets();
}

TEST(TestName, Join_Busy)
{
    InitializeSockets();
    JOIN_DEFAULT();

    n4gd::Connection busy(protocolId, timeOut);
    EXPECT_TRUE(busy.Start(clientPort + 1));
    busy.Connect(addrDefault);

    const PACKET b2s[] = "i'm so busy!";

    while(true)
    {
        if (!busy.IsConnecting() || busy.IsConnected())
            break;

        SendPacket(client, c2s);
        SendPacket(server, s2c);
        SendPacket(busy, b2s);
        ReceivePacket(client);
        ReceivePacket(server);
        ReceivePacket(busy);
        client.Update(deltaTime);
        server.Update(deltaTime);
        busy.Update(deltaTime);
        Wait(deltaTime);
    }

    EXPECT_TRUE(server.IsConnected());
    EXPECT_TRUE(client.IsConnected());
    EXPECT_TRUE(busy.ConnectFailed());
    EXPECT_FALSE(busy.IsConnected());

    ShutdownSockets();
}

TEST(TestName, Rejoin_Test) {
    InitializeSockets();
    JOIN_DEFAULT();

    while (client.IsConnected() || server.IsConnected())
    {
        ReceivePacket(client);
        ReceivePacket(server);
        client.Update(deltaTime);
        server.Update(deltaTime);
        Wait(deltaTime);
    }
    EXPECT_FALSE(client.IsConnected());
    EXPECT_FALSE(server.IsConnected());

    DEFAULT_TEST();
    ShutdownSockets();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

void SendPacket(n4gd::Connection& connection, const PacketString& packet)
{
    connection.SendPacket(packet.data(), packet.size());
}


void ReceivePacket(n4gd::Connection& connection)
{
    while (true) {
        PACKET packet[256];
        int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
        if (bytes_read == 0) {
            break;
        }
    }
}

void ReceivePacket(n4gd::Connection& connection,const PACKET expected[])
{
    while (true) {
        PACKET packet[256];
        int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
        if (bytes_read == 0) {
            break;
        }
        EXPECT_TRUE(strcmp(reinterpret_cast<const char *>(packet), reinterpret_cast<const char *>(expected)) == 0);
    }
}