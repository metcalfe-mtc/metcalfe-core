//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/chrono.h>
#include <mtchain/basics/KeyCache.h>
#include <mtchain/beast/unit_test.h>
#include <mtchain/beast/clock/manual_clock.h>

namespace mtchain {

class KeyCache_test : public beast::unit_test::suite
{
public:
    void run ()
    {
        TestStopwatch clock;
        clock.set (0);

        using Key = std::string;
        using Cache = KeyCache <Key>;

        // Insert an item, retrieve it, and age it so it gets purged.
        {
            Cache c ("test", clock, 1, 2);

            BEAST_EXPECT(c.size () == 0);
            BEAST_EXPECT(c.insert ("one"));
            BEAST_EXPECT(! c.insert ("one"));
            BEAST_EXPECT(c.size () == 1);
            BEAST_EXPECT(c.exists ("one"));
            BEAST_EXPECT(c.touch_if_exists ("one"));
            ++clock;
            c.sweep ();
            BEAST_EXPECT(c.size () == 1);
            BEAST_EXPECT(c.exists ("one"));
            ++clock;
            c.sweep ();
            BEAST_EXPECT(c.size () == 0);
            BEAST_EXPECT(! c.exists ("one"));
            BEAST_EXPECT(! c.touch_if_exists ("one"));
        }

        // Insert two items, have one expire
        {
            Cache c ("test", clock, 2, 2);

            BEAST_EXPECT(c.insert ("one"));
            BEAST_EXPECT(c.size  () == 1);
            BEAST_EXPECT(c.insert ("two"));
            BEAST_EXPECT(c.size  () == 2);
            ++clock;
            c.sweep ();
            BEAST_EXPECT(c.size () == 2);
            BEAST_EXPECT(c.touch_if_exists ("two"));
            ++clock;
            c.sweep ();
            BEAST_EXPECT(c.size () == 1);
            BEAST_EXPECT(c.exists ("two"));
        }

        // Insert three items (1 over limit), sweep
        {
            Cache c ("test", clock, 2, 3);

            BEAST_EXPECT(c.insert ("one"));
            ++clock;
            BEAST_EXPECT(c.insert ("two"));
            ++clock;
            BEAST_EXPECT(c.insert ("three"));
            ++clock;
            BEAST_EXPECT(c.size () == 3);
            c.sweep ();
            BEAST_EXPECT(c.size () < 3);
        }
    }
};

BEAST_DEFINE_TESTSUITE(KeyCache,common,mtchain);

}
