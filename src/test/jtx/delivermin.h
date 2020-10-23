//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_DELIVERMIN_H_INCLUDED
#define MTCHAIN_TEST_JTX_DELIVERMIN_H_INCLUDED

#include <test/jtx/Env.h>
#include <mtchain/protocol/STAmount.h>

namespace mtchain {
namespace test {
namespace jtx {

/** Sets the DeliverMin on a JTx. */
class delivermin
{
private:
    STAmount amount_;

public:
    delivermin (STAmount const& amount)
        : amount_(amount)
    {
    }

    void
    operator()(Env&, JTx& jtx) const;
};

} // jtx
} // test
} //

#endif
