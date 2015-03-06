/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include "SimpleNetworking.h"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
using TMessageLen = uint64_t;

void sn::SimpleConnection::send(string message)
{
    static_assert(sizeof(string::size_type) <= sizeof(TMessageLen), 
        "string::size_type > TMessageLen.");

    TMessageLen len = message.size();
    if (::send(_socket, &len, sizeof len, 0) == -1)
        throw runtime_error(strerror(errno));

    if (::send(_socket, message.data(), message.size(), 0) == -1)
        throw runtime_error(strerror(errno));
}

string sn::SimpleConnection::receive()
{
    TMessageLen len;
    if (::recv(_socket, &len, sizeof len, MSG_WAITALL) == -1)
        throw runtime_error(strerror(errno));

    string message(len, 0);
    if (::recv(_socket, &message[0], message.size(), MSG_WAITALL) == -1)
        throw runtime_error(strerror(errno));

    return message;
}

int sn::SimpleConnection::createSocket(array<uint8_t, 4> address, uint16_t port)
{
    int sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        throw runtime_error(strerror(errno));

    int optVal = 1;
    ::linger lingerOpt;
    lingerOpt.l_onoff = 1;
    lingerOpt.l_linger = 10;
    if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
            &optVal, sizeof optVal) == -1)
        throw runtime_error(strerror(errno));
    if (::setsockopt(sock, SOL_SOCKET, SO_LINGER, 
            &lingerOpt, sizeof lingerOpt) == -1)
        throw runtime_error(strerror(errno));

    ::sockaddr_in addressStruct;
    addressStruct.sin_family = AF_INET;
    addressStruct.sin_port = htons(port);
    addressStruct.sin_addr.s_addr =
        static_cast<uint32_t>(address[3]) << 24 |
        static_cast<uint32_t>(address[2]) << 16 |
        static_cast<uint32_t>(address[1]) << 8 |
        static_cast<uint32_t>(address[0]);

    if (::connect(sock, reinterpret_cast<sockaddr*>(&addressStruct),
            sizeof addressStruct) == -1)
        throw runtime_error(strerror(errno));

    return sock;
}

sn::SimpleConnection sn::SimpleAcceptor::accept()
{
    int sock = ::accept(_serverSocket, nullptr, nullptr);
    if (sock == -1)
        throw runtime_error(strerror(errno));

    return SimpleConnection(sock);
}

int sn::SimpleAcceptor::createSocket(uint16_t port)
{
    int sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        throw runtime_error(strerror(errno));

    int optVal = 1;
    ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof optVal);

    ::sockaddr_in addressStruct;
    addressStruct.sin_family = AF_INET;
    addressStruct.sin_port = htons(port);
    addressStruct.sin_addr.s_addr = INADDR_ANY;

    if (::bind(sock, reinterpret_cast<sockaddr*>(&addressStruct),
            sizeof addressStruct) == -1)
        throw runtime_error(strerror(errno));

    if (::listen(sock, SOMAXCONN) == -1)
        throw runtime_error(strerror(errno));

    return sock;
}

sn::SimpleConnection::~SimpleConnection()
{
    if (_socket >= 0)
        ::close(_socket);
}

sn::SimpleConnection::SimpleConnection(sn::SimpleConnection&& connection)
    : _socket(connection._socket)
{
    connection._socket = -1;
}

sn::SimpleConnection&
sn::SimpleConnection::operator=(sn::SimpleConnection&& connection)
{
    _socket = connection._socket;
    connection._socket = -1;
    return *this;
}
