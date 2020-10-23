//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/chrono.h>
#include <mtchain/peerfinder/impl/Livecache.h>
#include <mtchain/beast/unit_test.h>
#include <mtchain/beast/clock/manual_clock.h>

namespace mtchain {
namespace PeerFinder {

class Livecache_test : public beast::unit_test::suite
{
public:
    TestStopwatch m_clock;

    // Add the address as an endpoint
    template <class C>
    void add (std::uint32_t index, std::uint16_t port, C& c)
    {
        Endpoint ep;
        ep.hops = 0;
        ep.address = beast::IP::Endpoint (
            beast::IP::AddressV4 (index), port);
        c.insert (ep);
    }

    void testFetch ()
    {
        Livecache <> c (m_clock, beast::Journal());

        add (1, 1, c);
        add (2, 1, c);
        add (3, 1, c);
        add (4, 1, c);
        add (4, 2, c);
        add (4, 3, c);
        add (5, 1, c);
        add (6, 1, c);
        add (6, 2, c);
        add (7, 1, c);

        // VFALCO TODO!

        pass();
    }

    void run ()
    {
        testFetch ();
    }
};

BEAST_DEFINE_TESTSUITE(Livecache,peerfinder,mtchain);

}
}
