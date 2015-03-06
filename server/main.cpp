/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include "RpcConnection.h"

int main()
{
    sn::SimpleAcceptor acceptor(61000);
    while(true)
    {
        try
        {
            server::RpcConnection connection(acceptor.accept());
            connection.receiveCall();
            ::sleep(2);
        }
        catch(std::exception& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
        catch(...)
        {
            std::cerr << "Unknown error occurred.\n";
        }
    }
}

