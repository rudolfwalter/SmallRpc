/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#ifndef SIMPLE_NETWORKING_H
#define SIMPLE_NETWORKING_H
#include <array>
#include <cinttypes>

namespace sn
{
class SimpleConnection
{
    int _socket;
public:
    SimpleConnection(std::array<std::uint8_t, 4> address, std::uint16_t port)
        : _socket(createSocket(address, port))
    {
    }

    SimpleConnection(int socket)
        : _socket(socket)
    {
    }

    SimpleConnection(SimpleConnection&&);
    SimpleConnection& operator=(SimpleConnection&&);

    void send(std::string message);
    std::string receive();

    ~SimpleConnection();
private:
    int createSocket(std::array<std::uint8_t, 4> address, std::uint16_t port);
};


class SimpleAcceptor
{
    const int _serverSocket;
public:
    SimpleAcceptor(std::uint16_t listenPort)
        : _serverSocket(createSocket(listenPort))
    {}

    SimpleConnection accept();

private:
    int createSocket(std::uint16_t port);
};

}
#endif
