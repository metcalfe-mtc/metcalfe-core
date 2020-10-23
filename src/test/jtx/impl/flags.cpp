//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/flags.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

Json::Value
fset (Account const& account,
    std::uint32_t on, std::uint32_t off)
{
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::TransactionType] = "AccountSet";
    if (on != 0)
        jv[jss::SetFlag] = on;
    if (off != 0)
        jv[jss::ClearFlag] = off;
    return jv;
}

void
flags::operator()(Env& env) const
{
    auto const sle = env.le(account_);
    if (sle->isFieldPresent(sfFlags))
        env.test.expect((sle->getFieldU32(sfFlags) &
            mask_) == mask_);
    else
        env.test.expect(mask_ == 0);
}

void
nflags::operator()(Env& env) const
{
    auto const sle = env.le(account_);
    if (sle->isFieldPresent(sfFlags))
        env.test.expect((sle->getFieldU32(sfFlags) &
            mask_) == 0);
    else
        env.test.pass();
}

} // jtx
} // test
} //
