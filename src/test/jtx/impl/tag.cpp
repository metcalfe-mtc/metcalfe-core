//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/tag.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

void
dtag::operator()(Env&, JTx& jt) const
{
    jt.jv["DestinationTag"] = value_;
}

void
stag::operator()(Env&, JTx& jt) const
{
    jt.jv["SourceTag"] = value_;
}

} // jtx
} // test
} //
