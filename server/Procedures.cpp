/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include "Procedures.h"
#include <algorithm>

namespace server
{
namespace procedures
{

common::Integral add2(common::Integral a, common::Integral b)
{
    return common::Integral{a.value + b.value};
}

long sumLongs(std::vector<long> v)
{
    return std::accumulate(begin(v), end(v), 0L);
}

std::vector<float> doubleFloats(std::vector<float> v)
{
    for (float& x : v)
        x *= 2;
    return v;
}

std::string concat2(std::string s1, std::string s2)
{
    return s1 + s2;
}

}
}

