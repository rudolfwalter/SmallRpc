/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#ifndef CLIENT_RPC_CONNECTION_H
#define CLIENT_RPC_CONNECTION_H
#include <boost/optional.hpp>
#include "../common/SerializationHeaders.h"
#include "../common/RpcConnection.h"
#include "../common/SimpleNetworking.h"

namespace client
{

class RpcConnection
{
    sn::SimpleConnection _connection;
public:
    RpcConnection(sn::SimpleConnection connection)
        : _connection(std::move(connection))
    {
    }

    RpcConnection(RpcConnection&&) = default;
    RpcConnection& operator=(RpcConnection&&) = default;

    template <typename... T>
    void call(common::RemoteProcedureId procedureId, T... params)
    {
        std::ostringstream oss(std::ios::out | std::ios::binary);
        boost::archive::text_oarchive archive(oss);
        archive << procedureId;

        doCall(archive, params...);

        _connection.send(oss.str());
    }

    template <typename R>
    boost::optional<R> getResponse()
    {
        auto message = _connection.receive();
        std::istringstream iss(message, std::ios::in | std::ios::binary);
        boost::archive::text_iarchive archive(iss);

        common::CallResult callResult;
        archive >> callResult;
        if (callResult == common::CallResult::Failure)
            return boost::none;

        R result;
        archive >> result;
        return result;
    }

private:
    void doCall(boost::archive::text_oarchive&)
    {
    }

    template <typename T0, typename... T>
    typename std::enable_if<
        std::is_integral<typename std::remove_reference<T0>::type>::value, void
    >::type
    doCall(boost::archive::text_oarchive& archive, T0 t0, T const&... t)
    {
        archive << common::integral(t0);

        doCall(archive, t...);
    }

    template <typename T0, typename... T>
    typename std::enable_if<
        std::is_floating_point<typename std::remove_reference<T0>::type>::value,
        void
    >::type
    doCall(boost::archive::text_oarchive& archive, T0 t0, T const&... t)
    {
        archive << common::real(t0);

        doCall(archive, t...);
    }

    template <typename T0, typename... T>
    typename std::enable_if<
        !std::is_integral<typename std::remove_reference<T0>::type>::value &&
        !std::is_floating_point<typename std::remove_reference<T0>::type>::value
        ,void
    >::type
    doCall(boost::archive::text_oarchive& archive, T0 const& t0, T const&... t)
    {
        archive << t0;

        doCall(archive, t...);
    }
};

void currentRpcConnection(RpcConnection c);
RpcConnection& currentRpcConnection();

}

#endif
