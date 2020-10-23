//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_BALANCE_H_INCLUDED
#define MTCHAIN_TEST_JTX_BALANCE_H_INCLUDED

#include <test/jtx/tags.h>
#include <test/jtx/Env.h>

namespace mtchain {
namespace test {
namespace jtx {

/** A balance matches.

    This allows "none" which means either the account
    doesn't exist (no M) or the trust line does not
    exist. If an amount is specified, the SLE must
    exist even if the amount is 0, or else the test
    fails.
*/
class balance
{
private:
    bool none_;
    Account account_;
    STAmount value_;

public:
    balance (Account const& account,
            none_t)
        : none_(true)
        , account_(account)
        , value_(M)
    {
    }

    balance (Account const& account,
            None const& value)
        : none_(true)
        , account_(account)
        , value_(value.issue)
    {
    }

    balance (Account const& account,
            STAmount const& value)
        : none_(false)
        , account_(account)
        , value_(value)
    {
    }

    void
    operator()(Env&) const;
};

} // jtx
} // test
} //

#endif
