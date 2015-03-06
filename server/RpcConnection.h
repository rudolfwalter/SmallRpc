/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#ifndef SERVER_RPC_CONNECTION_H
#define SERVER_RPC_CONNECTION_H
#include "../common/SerializationHeaders.h"
#include "../common/RpcConnection.h"
#include "../common/SimpleNetworking.h"
#include "Procedures.h"

namespace server
{

class RpcConnection
{
    sn::SimpleConnection _connection;
public:
    RpcConnection(sn::SimpleConnection connection)
        : _connection(std::move(connection))
    {
    }

    void receiveCall();

private:
    void dispatchCall(boost::archive::text_iarchive& receiveArchive,
        boost::archive::text_oarchive& responseArchive);

    template <typename R, typename F>
    R doReceiveCall(boost::archive::text_iarchive&, F f)
    {
        return f();
    }

    template <typename R, typename F, typename T0, typename... T>
    R doReceiveCall(boost::archive::text_iarchive& archive, F f)
    {
        T0 x;
        archive >> x;
        auto ff = [&f, &x](T... t){return f(x, t...);};

        return doReceiveCall<R, decltype(ff), T...>(archive, ff);
    }

    template <typename R, typename... T>
    R receiveCall(boost::archive::text_iarchive& archive, R f(T...))
    {
        return doReceiveCall<R, decltype(f), T...>(archive, f);
    }
};

}

#endif

