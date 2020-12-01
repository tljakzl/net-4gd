//Ѫ Ӕ Ѫ

#include "Ipv4.h"
#include "UdpSocket.h"

namespace n4gd
{
    class Connection
    {
    public:
        enum class Mode{
            None = 0,
            Client,
            Server
        };

        Connection() = delete;
        Connection(Ipv4::ADDRESS protocol, float timeout);
        ~Connection();
        bool Start(Ipv4::PORT port);
        void Stop();
        void Listen();
        void Connect(const Ipv4& address);
        void Update(float dt);

        bool IsConnecting() const;
        bool ConnectFailed() const;
        bool IsConnected() const;
        bool IsListening() const;
        bool SendPacket(const unsigned char data[], int size);
        int ReceivePacket(unsigned char data[], int size);
        Mode GetMode() const;

    protected:

        void ClearData();

    private:

        enum class State
        {
            Disconnected = 0,
            Listening,
            Connecting,
            ConnectFail,
            Connected
        };

        Ipv4::ADDRESS _protocol{0};
        float _timeout{0.f};
        float _timeoutSum{0.f};
        Mode _mode{Mode::None};
        State _state{State::Disconnected};
        UdpSocket _socket{};
        Ipv4 _address{};
        bool _running{false};

    };
}

