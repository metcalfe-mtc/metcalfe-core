//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/rate.h>
#include <mtchain/protocol/JsonFields.h>
#include <mtchain/basics/contract.h>
#include <stdexcept>

namespace mtchain {
namespace test {
namespace jtx {

Json::Value
rate (Account const& account, double multiplier)
{
    if (multiplier > 4)
        Throw<std::runtime_error> (
            "rate multiplier out of range");
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::TransferRate] = std::uint32_t(
        1000000000 * multiplier);
    jv[jss::TransactionType] = "AccountSet";
    return jv;
}

} // jtx
} // test
} //
