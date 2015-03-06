/* This file is part of RWG's SmallRpc.
Copyright 2015 Rudolf-Walter Kiss-Szakacs
See the COPYING file for licensing information.*/

#include <iostream>
#include <iterator>
#include "RpcConnection.h"
#include "Procedures.h"
using namespace std;
using namespace client::procedures;

int main()
{
    client::currentRpcConnection(
        client::RpcConnection (sn::SimpleConnection({{127,0,0,1}}, 61000)));

    cout << " Menu:\n"
            " [1.] - Add 2 numbers\n"
            " [2.] - Sum some longs\n"
            " [3.] - Double some floats\n"
            " [4.] - Concatenate 2 lines\n\n"
            ">> ";
    int menu;
    cin >> menu;
    switch(menu)
    {
        case 1:
        {
            int x, y;
            cin >> x >> y;
            cout << x << " + " << y << " = " << add2(x, y);
            break;
        }
        case 2:
        {
            vector<long> v;
            copy(istream_iterator<long>(cin), istream_iterator<long>(), 
                back_inserter(v));
            cout << "Sum = " << sumLongs(v);
            break;
        }
        case 3:
        {
            vector<float> v;
            copy(istream_iterator<float>(cin), istream_iterator<float>(),
                back_inserter(v));
            v = doubleF(v);
            copy(begin(v), end(v), ostream_iterator<float>(cout, " "));
            break;
        }
        case 4:
        {
            string s1, s2;
            cin.ignore(10000, '\n');
            getline(cin, s1);
            getline(cin, s2);
            cout << cat(s1, s2);
            break;
        }
    }
    cout << endl;
}
