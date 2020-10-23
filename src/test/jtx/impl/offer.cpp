//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/offer.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

Json::Value
offer (Account const& account,
    STAmount const& in, STAmount const& out)
{
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::TakerPays] = in.getJson(0);
    jv[jss::TakerGets] = out.getJson(0);
    jv[jss::TransactionType] = "OfferCreate";
    return jv;
}

} // jtx
} // test
} //
