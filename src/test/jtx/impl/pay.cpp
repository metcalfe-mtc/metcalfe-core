//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/pay.h>
#include <mtchain/protocol/JsonFields.h>
#include <mtchain/protocol/TxFlags.h>

namespace mtchain {
namespace test {
namespace jtx {

Json::Value
pay (Account const& account,
    Account const& to,
        AnyAmount amount)
{
    amount.to(to);
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::Amount] = amount.value.getJson(0);
    jv[jss::Destination] = to.human();
    jv[jss::TransactionType] = "Payment";
    jv[jss::Flags] = tfUniversal;
    return jv;
}

} // jtx
} // test
} //
