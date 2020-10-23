//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/RangeSet.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

class RangeSet_test : public beast::unit_test::suite
{
public:
    RangeSet createPredefinedSet ()
    {
        RangeSet set;

        // Set will include:
        // [ 0, 5]
        // [10,15]
        // [20,25]
        // etc...

        for (int i = 0; i < 10; ++i)
            set.setRange (10 * i, 10 * i + 5);

        return set;
    }

    void testMembership ()
    {
        testcase ("membership");

        RangeSet r1, r2;

        r1.setRange (1, 10);
        r1.clearValue (5);
        r1.setRange (11, 20);

        r2.setRange (1, 4);
        r2.setRange (6, 10);
        r2.setRange (10, 20);

        BEAST_EXPECT(!r1.hasValue (5));

        BEAST_EXPECT(r2.hasValue (9));
    }

    void testPrevMissing ()
    {
        testcase ("prevMissing");

        RangeSet const set = createPredefinedSet ();

        for (int i = 0; i < 100; ++i)
        {
            int const oneBelowRange = (10*(i/10))-1;

            int const expectedPrevMissing =
                ((i % 10) > 6) ? (i-1) : oneBelowRange;

            BEAST_EXPECT(set.prevMissing (i) == expectedPrevMissing);
        }
    }

    void run ()
    {
        testMembership ();

        testPrevMissing ();

        // TODO: Traverse functions must be tested
    }
};

BEAST_DEFINE_TESTSUITE(RangeSet,mtchain_basics,mtchain);

} //

