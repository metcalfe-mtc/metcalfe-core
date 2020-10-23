//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/protocol/types.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

struct types_test : public beast::unit_test::suite
{
    void
    testAccountID()
    {
        auto const s =
            "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh";
        if (BEAST_EXPECT(parseBase58<AccountID>(s)))
            BEAST_EXPECT(toBase58(
                *parseBase58<AccountID>(s)) == s);
    }

    void
    run() override
    {
        testAccountID();
    }
};

BEAST_DEFINE_TESTSUITE(types,protocol,mtchain);

}
