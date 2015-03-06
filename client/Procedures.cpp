/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include "Procedures.h"
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
using namespace common;

namespace client
{
namespace procedures
{

long sumLongs(std::vector<long> v)
{
    client::currentRpcConnection().call(RemoteProcedureId::SumLongs, v);

    auto result = client::currentRpcConnection().getResponse<long>();
    if (!result) throw std::runtime_error("Call failed");
    return *result;
}

std::vector<float> doubleF(std::vector<float> v)
{
    client::currentRpcConnection().call(RemoteProcedureId::DoubleFloats, v);

    auto result = client::currentRpcConnection().getResponse<vector<float>>();
    if (!result) throw std::runtime_error("Call failed");
    return *result;
}


std::string cat(std::string s1, std::string s2)
{
    client::currentRpcConnection().call(
        RemoteProcedureId::ConcatTwoStrings, s1, s2);

    auto result = client::currentRpcConnection().getResponse<string>();
    if (!result) throw std::runtime_error("Call failed");
    return *result;
}

}
}
