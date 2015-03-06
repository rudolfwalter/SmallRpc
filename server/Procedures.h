/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#ifndef PROCEDURES_H
#define PROCEDURES_H
#include "../common/RpcConnection.h"
#include <vector>
#include <string>

namespace server
{
namespace procedures
{

common::Integral add2(common::Integral a, common::Integral b);

long sumLongs(std::vector<long> v);

std::vector<float> doubleFloats(std::vector<float> v);

std::string concat2(std::string s1, std::string s2);

}
}

#endif

