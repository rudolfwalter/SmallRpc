/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include "RpcConnection.h"
#include <memory>

static std::unique_ptr<client::RpcConnection> _currentConnection = nullptr;


void ::client::currentRpcConnection(client::RpcConnection c)
{
    _currentConnection = std::make_unique<client::RpcConnection>(std::move(c));
}

client::RpcConnection& ::client::currentRpcConnection()
{
    if (!_currentConnection)
        throw std::runtime_error("No connection");

    return *_currentConnection;
}
