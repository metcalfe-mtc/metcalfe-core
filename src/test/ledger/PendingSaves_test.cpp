//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.
    Permission to use, copy, modify, and/or distribute this software for any
*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/app/ledger/PendingSaves.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {
namespace test {

struct PendingSaves_test : public beast::unit_test::suite
{
    void testSaves()
    {
        PendingSaves ps;

        // Basic test
        BEAST_EXPECT(!ps.pending (0));
        BEAST_EXPECT(!ps.startWork(0));
        BEAST_EXPECT(ps.shouldWork (0, true));
        BEAST_EXPECT(ps.startWork (0));
        BEAST_EXPECT(ps.pending (0));
        BEAST_EXPECT(! ps.shouldWork (0, false));
        ps.finishWork(0);
        BEAST_EXPECT(!ps.pending (0));

        // Test work stealing
        BEAST_EXPECT(ps.shouldWork (0, false));
        BEAST_EXPECT(ps.pending (0));
        BEAST_EXPECT(ps.shouldWork (0, true));
        BEAST_EXPECT(ps.pending (0));
        BEAST_EXPECT(ps.startWork (0));
        BEAST_EXPECT(!ps.startWork (0));
        ps.finishWork(0);
        BEAST_EXPECT(! ps.pending (0));
    }

    void run() override
    {
        testSaves();
    }
};

BEAST_DEFINE_TESTSUITE(PendingSaves,ledger,mtchain);

} // test
} //
