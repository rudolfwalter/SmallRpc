/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include "RpcConnection.h"
#include <vector>
#include <sstream>
#include <stdexcept>

void server::RpcConnection::receiveCall()
{
    auto message = _connection.receive();
    //std::cerr << message << '\n';
    std::istringstream iss(message, std::ios::in | std::ios::binary);
    boost::archive::text_iarchive archive(iss);

    std::ostringstream oss(std::ios::out | std::ios::binary);
    boost::archive::text_oarchive responseArchive(oss);

    dispatchCall(archive, responseArchive);

    auto response = oss.str();
    //std::cerr << response << '\n';
    _connection.send(response);
}

#define DISPATCH_CASE(f) { \
    auto result = receiveCall(receiveArchive, procedures:: f ); \
    responseArchive << callResult; \
    responseArchive << result; \
} break

void server::RpcConnection::dispatchCall(
    boost::archive::text_iarchive& receiveArchive, 
    boost::archive::text_oarchive& responseArchive)
{
    using ProcId = common::RemoteProcedureId;
    common::CallResult callResult = common::CallResult::Success;
    try
    {
        common::RemoteProcedureId procedureId;
        receiveArchive >> procedureId;

        switch(procedureId)
        {
            case ProcId::AddTwoIntegrals:  DISPATCH_CASE(add2);
            case ProcId::SumLongs:         DISPATCH_CASE(sumLongs);
            case ProcId::DoubleFloats:     DISPATCH_CASE(doubleFloats);
            case ProcId::ConcatTwoStrings: DISPATCH_CASE(concat2);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcovered-switch-default"
            default: throw std::runtime_error("Invalid procedure id.");
#pragma clang diagnostic pop
        };
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        callResult = common::CallResult::Failure;
        responseArchive << callResult;
    }
    catch(...)
    {
        callResult = common::CallResult::Failure;
        responseArchive << callResult;
    }
}

#undef DISPATCH_CASE

