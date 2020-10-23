//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/app/tx/impl/OfferStream.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

class OfferStream_test : public beast::unit_test::suite
{
public:
    void
    test()
    {
        pass();
    }

    void
    run()
    {
        test();
    }
};

BEAST_DEFINE_TESTSUITE(OfferStream,tx,mtchain);

}
