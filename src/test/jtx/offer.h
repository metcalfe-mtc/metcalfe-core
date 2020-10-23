//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_OFFER_H_INCLUDED
#define MTCHAIN_TEST_JTX_OFFER_H_INCLUDED

#include <test/jtx/Account.h>
#include <mtchain/json/json_value.h>
#include <mtchain/protocol/STAmount.h>

namespace mtchain {
namespace test {
namespace jtx {

/** Create an offer. */
Json::Value
offer (Account const& account,
    STAmount const& in, STAmount const& out);

} // jtx
} // test
} //

#endif
