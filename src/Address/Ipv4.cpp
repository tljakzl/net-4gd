//Ѫ Ӕ Ѫ

#include <n4gd/Ipv4.h>

#define OCTED_BYTE 8

constexpr Ipv4::OCTET OFFSET_A = 3 * OCTED_BYTE;
constexpr Ipv4::OCTET OFFSET_B = 2 * OCTED_BYTE;
constexpr Ipv4::OCTET OFFSET_C = 1 * OCTED_BYTE;

Ipv4::Ipv4(Ipv4::OCTET a, Ipv4::OCTET b, Ipv4::OCTET c, Ipv4::OCTET d, Ipv4::PORT port)
: _address((a << OFFSET_A ) | ( b << OFFSET_B ) | ( c << OFFSET_C ) | d)
, _port(port)
{
}

Ipv4::Ipv4(Ipv4::ADDRESS address, Ipv4::PORT port)
: _address(address)
, _port(port)
{
}

Ipv4::ADDRESS Ipv4::GetAddress() const
{
    return _address;
}

Ipv4::PORT Ipv4::GetPort() const
{
    return _port;
}

bool Ipv4::operator==(const Ipv4 &other) const
{
    return _address == other._address && _port == other._port;
}

bool Ipv4::operator!=(const Ipv4 &other) const
{
    return !(*this == other);
}

Ipv4::OCTET Ipv4::GetA() const
{
    return _address >> OFFSET_A;
}

Ipv4::OCTET Ipv4::GetB() const
{
    return _address >> OFFSET_B;
}

Ipv4::OCTET Ipv4::GetC() const
{
    return _address >> OFFSET_C;
}

Ipv4::OCTET Ipv4::GetD() const
{
    return _address;
}
