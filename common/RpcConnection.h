/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#ifndef COMMON_RPC_CONNECTION_H
#define COMMON_RPC_CONNECTION_H
#include <cinttypes>
#include <type_traits>

namespace common
{
enum class RemoteProcedureId
{
    AddTwoIntegrals,
    SumLongs,
    DoubleFloats,
    ConcatTwoStrings
};

enum class CallResult
{
    Success,
    Failure
};

struct Integral
{
    std::intmax_t value;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned) {
        ar & value;
    }
};

struct Real
{
    long double value;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned) {
        ar & value;
    }
};

template<typename T>
typename std::enable_if<std::is_integral<T>::value, Integral>::type
integral(T val)
{
    return Integral{val};
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, Real>::type
real(T val)
{
    return Real{val};
}

}

#endif
