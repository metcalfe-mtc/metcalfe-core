//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/trust.h>
#include <mtchain/protocol/JsonFields.h>
#include <mtchain/basics/contract.h>
#include <stdexcept>

namespace mtchain {
namespace test {
namespace jtx {

Json::Value
trust (Account const& account,
    STAmount const& amount,
        std::uint32_t flags)
{
    if (isM(amount))
        Throw<std::runtime_error> (
            "trust() requires IOU");
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::LimitAmount] = amount.getJson(0);
    jv[jss::TransactionType] = "TrustSet";
    jv[jss::Flags] = flags;
    return jv;
}

Json::Value
trust (Account const& account,
    STAmount const& amount,
    Account const& peer,
    std::uint32_t flags)
{
    if (isM(amount))
        Throw<std::runtime_error> (
            "trust() requires IOU");
    Json::Value jv;
    jv[jss::Account] = account.human();
    {
        auto& ja = jv[jss::LimitAmount] = amount.getJson(0);
        ja[jss::issuer] = peer.human();
    }
    jv[jss::TransactionType] = "TrustSet";
    jv[jss::Flags] = flags;
    return jv;
}


} // jtx
} // test
} //
