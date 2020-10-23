//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_REGKEY_H_INCLUDED
#define MTCHAIN_TEST_JTX_REGKEY_H_INCLUDED

#include <test/jtx/Account.h>
#include <test/jtx/tags.h>
#include <mtchain/json/json_value.h>

namespace mtchain {
namespace test {
namespace jtx {

/** Disable the regular key. */
Json::Value
regkey (Account const& account,
    disabled_t);

/** Set a regular key. */
Json::Value
regkey (Account const& account,
    Account const& signer);

} // jtx
} // test
} //

#endif
