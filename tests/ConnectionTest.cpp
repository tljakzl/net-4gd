//Ѫ Ӕ Ѫ

#include <n4gd/Connection.h>
#include <n4gd/Utils.h>
#include <gtest/gtest.h>
#include <bitset>
#include <fstream>

typedef unsigned char PACKET;
typedef std::basic_string<PACKET, std::char_traits<PACKET>, std::allocator<PACKET>> PacketString;

void SendPacket(n4gd::Connection& connection, const PacketString& packet);
void ReceivePacket(n4gd::Connection& connection, const PACKET expected[]);

constexpr int serverPort = 30000;
constexpr int clientPort = 30001;
constexpr int protocolId = 0x11112222;
constexpr float deltaTime = 0.001f;
constexpr float timeOut = 0.1f;

constexpr PACKET pNull[] = "";
constexpr PACKET c2s[] = "client to server";
constexpr PACKET s2c[] = "server to client";

static const Ipv4 addrDefault(127, 0, 0, 1, serverPort);

void DefaultTest(n4gd::Connection& client, n4gd::Connection& server)
{
    client.Connect(addrDefault);
    while (true) {
        if (client.IsConnected() && server.IsConnected()) {
            break;
        }
        if (client.ConnectFailed() || server.ConnectFailed()) {
            break;
        }
        SendPacket(client, c2s);
        SendPacket(server, s2c);
        ReceivePacket(client, s2c);
        ReceivePacket(server, c2s);
        client.Update(deltaTime);
        server.Update(deltaTime);
        Wait(deltaTime);
    }
    EXPECT_TRUE(server.IsConnected());
    EXPECT_TRUE(client.IsConnected());
}

TEST(Connection, Join_Test)
{
    InitializeSockets();

    n4gd::Connection client(protocolId, timeOut);
    n4gd::Connection server(protocolId, timeOut);
    EXPECT_TRUE(client.Start(clientPort));
    EXPECT_TRUE(server.Start(serverPort));
    server.Listen();
    DefaultTest(client, server);

    ShutdownSockets();
}

TEST(Connection, Join_Time_Out)
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
        ReceivePacket(client, pNull);
        client.Update(deltaTime);
        Wait(deltaTime);
    }
    EXPECT_FALSE(client.IsConnected());
    EXPECT_TRUE(client.ConnectFailed());
    ShutdownSockets();
}

TEST(Connection, Join_Busy)
{
    InitializeSockets();

    n4gd::Connection client(protocolId, timeOut);
    n4gd::Connection server(protocolId, timeOut);
    EXPECT_TRUE(client.Start(clientPort));
    EXPECT_TRUE(server.Start(serverPort));
    server.Listen();
    DefaultTest(client, server);

    n4gd::Connection busy(protocolId, timeOut);
    EXPECT_TRUE(busy.Start(clientPort + 1));
    busy.Connect(addrDefault);

    constexpr PACKET b2s[] = "i'm so busy!";

    while(true)
    {
        if (!busy.IsConnecting() || busy.IsConnected())
            break;

        SendPacket(client, c2s);
        SendPacket(server, s2c);
        SendPacket(busy, b2s);
        ReceivePacket(client, s2c);
        ReceivePacket(server, c2s);
        ReceivePacket(busy, b2s);
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

TEST(Connection, Rejoin_Test) {
    InitializeSockets();

    n4gd::Connection client(protocolId, timeOut);
    n4gd::Connection server(protocolId, timeOut);
    EXPECT_TRUE(client.Start(clientPort));
    EXPECT_TRUE(server.Start(serverPort));
    server.Listen();
    DefaultTest(client, server);

    while (client.IsConnected() || server.IsConnected())
    {
        ReceivePacket(client,c2s);
        ReceivePacket(server,s2c);
        client.Update(deltaTime);
        server.Update(deltaTime);
        Wait(deltaTime);
    }
    EXPECT_FALSE(client.IsConnected());
    EXPECT_FALSE(server.IsConnected());

    DefaultTest(client, server);
    ShutdownSockets();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    std::ofstream filestream("log.txt");
    std::cout.rdbuf(filestream.rdbuf());

    return RUN_ALL_TESTS();
}

void SendPacket(n4gd::Connection& connection, const PacketString& packet)
{
    connection.SendPacket(packet.data(), packet.size());
}

void ReceivePacket(n4gd::Connection& connection, const PACKET expected[])
{
    std::vector<PACKET> packet(256);
    while (true) {
        int bytes_read = connection.ReceivePacket(packet.data(), packet.size());
        if (bytes_read == 0) {
            break;
        }
        EXPECT_TRUE(strcmp(reinterpret_cast<const char *>(packet.data()), reinterpret_cast<const char *>(expected)) == 0);
    }
}