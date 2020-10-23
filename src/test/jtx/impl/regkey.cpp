//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/regkey.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

Json::Value
regkey (Account const& account,
    disabled_t)
{
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::TransactionType] = "SetRegularKey";
    return jv;
}

Json::Value
regkey (Account const& account,
    Account const& signer)
{
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv["RegularKey"] = to_string(signer.id());
    jv[jss::TransactionType] = "SetRegularKey";
    return jv;
}

} // jtx
} // test
} //
