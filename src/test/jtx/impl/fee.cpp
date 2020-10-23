//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/fee.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

void
fee::operator()(Env&, JTx& jt) const
{
    if (! manual_)
        return;
    jt.fill_fee = false;
    if (amount_)
        jt[jss::Fee] =
            amount_->getJson(0);
}

} // jtx
} // test
} //
