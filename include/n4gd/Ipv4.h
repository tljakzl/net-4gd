//Ѫ Ӕ Ѫ

#pragma once

#include <cstdint>
#include <string>

class Ipv4{

public:

    typedef uint8_t OCTET;
    typedef uint32_t ADDRESS;
    typedef uint32_t PORT;

public:

#define OCTED_BYTE 8

    static const Ipv4::OCTET OFFSET_A = 3 * OCTED_BYTE;
    static const Ipv4::OCTET OFFSET_B = 2 * OCTED_BYTE;
    static const Ipv4::OCTET OFFSET_C = 1 * OCTED_BYTE;
public:
    Ipv4() = default;
    Ipv4(OCTET a, OCTET b, OCTET c , OCTET d, PORT port);
    Ipv4(ADDRESS address, PORT port);

    OCTET GetA() const;
    OCTET GetB() const;
    OCTET GetC() const;
    OCTET GetD() const;
    ADDRESS GetAddress() const;
    PORT GetPort() const;
    std::string GetAddressString() const;

    bool operator== (const Ipv4& other) const;
    bool operator!= (const Ipv4& other) const;

private:
    ADDRESS _address{0};
    PORT _port{0};
};

