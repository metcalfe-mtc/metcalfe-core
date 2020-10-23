//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/sig.h>
#include <test/jtx/utility.h>

namespace mtchain {
namespace test {
namespace jtx {

void
sig::operator()(Env&, JTx& jt) const
{
    if (! manual_)
        return;
    jt.fill_sig = false;
    if(account_)
    {
        // VFALCO Inefficient pre-C++14
        auto const account = *account_;
        jt.signer = [account](Env&, JTx& jt)
        {
            jtx::sign(jt.jv, account);
        };
    }
}

} // jtx
} // test
} //
