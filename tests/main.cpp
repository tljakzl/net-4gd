//Ѫ Ӕ Ѫ

#include <n4gd/Ipv4.h>
#include <n4gd/UdpSocket.h>
#include <gtest/gtest.h>

TEST(TestName, AddressTestDefault)
{
    Ipv4 address;
    ASSERT_EQ(address.GetAddress(), 0);
    ASSERT_EQ(address.GetPort(), 0);

    ASSERT_EQ(address.GetA(), 0);
    ASSERT_EQ(address.GetB(), 0);
    ASSERT_EQ(address.GetC(), 0);
    ASSERT_EQ(address.GetD(), 0);
}

TEST(TestName, AddressTestRandom)
{
    Ipv4::OCTET a = 100;
    Ipv4::OCTET b = 110;
    Ipv4::OCTET c = 50;
    Ipv4::OCTET d = 12;
    Ipv4::PORT port = 10000;
    Ipv4 address(a, b, c, d, port);
    ASSERT_EQ(port, address.GetPort());
    ASSERT_EQ(a, address.GetA());
    ASSERT_EQ(b, address.GetB());
    ASSERT_EQ(c, address.GetC());
    ASSERT_EQ(d, address.GetD());
}

TEST(TestName, AddressTestTwoAddress)
{
    Ipv4 x(100, 110, 0, 1, 50000);
    Ipv4 y(101, 210, 6, 5, 50002);
    ASSERT_NE(x, y);
    ASSERT_EQ(y, y);
    ASSERT_EQ(x, x);
}

TEST(TestName, UdpSocketOpenClose)
{
    InitializeSockets();
    UdpSocket socket;
    ASSERT_FALSE(socket.IsOpen());
    ASSERT_TRUE(socket.Open(30000));
    ASSERT_TRUE(socket.IsOpen());
    socket.Close();
    ASSERT_FALSE(socket.IsOpen());
    ASSERT_TRUE(socket.Open(30000));
    ASSERT_TRUE(socket.IsOpen());
    ShutdownSockets();
}
TEST(TestName, UdpSocketSamePorts)
{
    InitializeSockets();
    UdpSocket a, b;
    ASSERT_TRUE(a.Open(30000));
    ASSERT_FALSE(b.Open(30000));
    ASSERT_TRUE(a.IsOpen());
    ASSERT_FALSE(b.IsOpen());
    ShutdownSockets();
}
TEST(TestName, UdpSocketSendResive)
{
    InitializeSockets();
    UdpSocket a, b;
    ASSERT_TRUE(a.Open(30000));
    ASSERT_TRUE(b.Open(30001));
    const char packet[] = "packet data";
    bool a_received_packet = false;
    bool b_received_packet = false;
    while (!a_received_packet && !b_received_packet)
    {
        ASSERT_TRUE(a.Send(Ipv4(127, 0, 0, 1, 30000), packet, sizeof(packet)));
        ASSERT_TRUE(b.Send(Ipv4(127, 0, 0, 1, 30000), packet, sizeof(packet)));
        while (true)
        {
            Ipv4 sender;
            char buffer[256];
            int bytes_read = a.Receive(sender, buffer, sizeof(buffer));
            if (bytes_read == 0)
                break;
            if (bytes_read == sizeof(packet) && strcmp(buffer, packet) == 0)
                a_received_packet = true;
        }

        while (true)
        {
            Ipv4 sender;
            char buffer[256];
            int bytes_read = b.Receive(sender, buffer, sizeof(buffer));
            if (bytes_read == 0)
                break;
            if (bytes_read == sizeof(packet) && strcmp(buffer, packet) == 0)
                b_received_packet = true;
        }
    }
    ShutdownSockets();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
