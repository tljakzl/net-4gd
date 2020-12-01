//Ѫ Ӕ Ѫ

#include <n4gd/pch.h>
#include <n4gd/Connection.h>

namespace n4gd
{
    Connection::Connection(Ipv4::ADDRESS protocol, float timeout)
    : _protocol(protocol)
    , _timeout(timeout)
    {
    }

    Connection::~Connection()
    {
        if (_running) {
            Stop();
        }
    }

    bool Connection::Start(Ipv4::PORT port)
    {
        assert(!_running);
        std::cout << "Start connection on port: " << port << std::endl;
        if (!_socket.Open(port)) {
            return false;
        }
        _running = true;
        return true;
    }

    void Connection::Stop()
    {
        assert(_running);
        std::cout << "Stop connection on port: " << _socket.GetPort() << std::endl;
        ClearData();
        _socket.Close();
        _running = false;
    }

    void Connection::Listen()
    {
        std::cout << "Listening for connection on port: " << _socket.GetPort() << std::endl;
        ClearData();
        _mode = Mode::Server;
        _state = State::Listening;
    }

    void Connection::Connect(const Ipv4 &address)
    {
        std::cout << "Client connection to: " << address.GetAddressString();
        ClearData();
        _mode = Mode::Client;
        _state = State::Connecting;
        _address = address;
    }

    bool Connection::IsConnected() const
    {
        return _state == State::Connected;
    }

    bool Connection::ConnectFailed() const
    {
        return _state == State::ConnectFail;
    }

    bool Connection::IsConnecting() const
    {
        return _state == State::Connecting;
    }

    bool Connection::IsListening() const
    {
        return _state == State::Listening;
    }

    Connection::Mode Connection::GetMode() const
    {
        return _mode;
    }

    void Connection::Update(float dt)
    {
        assert(_running);
        _timeoutSum += dt;
        if (_timeoutSum > _timeout) {
            if (_state == State::Connecting) {
                std::cout << "Connect timed out\n";
                ClearData();
                _state = State::ConnectFail;
            }
            else if (_state == State::Connected) {
               std::cout << "Connection timed out\n";
                ClearData();
                if (_state == State::Connecting)
                    _state = State::ConnectFail;
            }
        }
    }

    void Connection::ClearData()
    {
        _state = State::Disconnected;
        _timeoutSum = 0.f;
        _address = Ipv4();
    }

    bool Connection::SendPacket(const unsigned char *data, int size)
    {
        assert(_running);
        if(_address.GetAddress() == 0) {
            return false;
        }
        unsigned char packet[size+4];
        packet[0] = static_cast<Ipv4::OCTET>(_protocol >> Ipv4::OFFSET_A);
        packet[1] = static_cast<Ipv4::OCTET>(_protocol >> Ipv4::OFFSET_B);
        packet[2] = static_cast<Ipv4::OCTET>(_protocol >> Ipv4::OFFSET_C);
        packet[3] = static_cast<Ipv4::OCTET>(_protocol);
        memcpy(&packet[4], data, size);
        return _socket.Send(_address, packet, size + 4);
    }

    int Connection::ReceivePacket(unsigned char *data, int size)
    {
        assert(_running);
        unsigned char packet[size + 4];
        Ipv4 sender;
        int bytesRead = _socket.Receive(sender, packet, size + 4);

        if (bytesRead == 0) {
            return 0;
        }
        if (bytesRead <= 4) {
            return 0;
        }

        if (packet[0] != static_cast<Ipv4::OCTET>(_protocol >> Ipv4::OFFSET_A) ||
        packet[1] != static_cast<Ipv4::OCTET>(_protocol >> Ipv4::OFFSET_B) ||
        packet[2] != static_cast<Ipv4::OCTET>(_protocol >> Ipv4::OFFSET_C) ||
        packet[3] != static_cast<Ipv4::OCTET>(_protocol)) {
            return 0;
        }

        if (_mode == Mode::Server && !IsConnected()) {
            std::cout << "Server accepts connection from client: " << sender.GetAddressString() << std::endl;
            _state = State::Connected;
            _address = sender;
        }

        if (sender == _address) {
            if(_mode == Mode::Client && _state == State::Connecting) {
                std::cout << "Client completes connection with server: " << sender.GetAddressString() << std::endl;
                _state = State::Connected;
            }
            _timeoutSum = 0.f;
            memcpy(data, &packet[4], size - 4);
            return size - 4;
        }
        return 0;
    }
}

