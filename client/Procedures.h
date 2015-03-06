/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#ifndef PROCEDURES_H
#define PROCEDURES_H
#include "../common/RpcConnection.h"
#include "RpcConnection.h"
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <string>

namespace client
{
namespace procedures
{

template <typename T, typename U>
typename std::common_type<T,U>::type add2(T a, U b)
{
    using namespace common;
    client::currentRpcConnection().call(
        RemoteProcedureId::AddTwoIntegrals, integral(a), integral(b));

    boost::optional<Integral> result =
        client::currentRpcConnection().getResponse<Integral>();
    
    if (!result) throw std::runtime_error("Call failed");
    return static_cast<typename std::common_type<T,U>::type>(result->value);
}

long sumLongs(std::vector<long> v);

std::vector<float> doubleF(std::vector<float> v);

std::string cat(std::string s1, std::string s2);

}
}

#endif
