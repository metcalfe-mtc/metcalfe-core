//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/jtx_json.h>
#include <test/jtx/utility.h>
#include <mtchain/json/json_reader.h>
#include <mtchain/basics/contract.h>

namespace mtchain {
namespace test {
namespace jtx {

json::json(std::string const& s)
{
    if (! Json::Reader().parse(s, jv_))
        Throw<parse_error> ("bad json");

}

json::json (char const* s)
    : json(std::string(s)){}

json::json (Json::Value jv)
    : jv_ (std::move (jv))
{
}

void
json::operator()(Env&, JTx& jt) const
{
    auto& jv = jt.jv;
    for (auto iter = jv_.begin();
            iter != jv_.end(); ++iter)
        jv[iter.key().asString()] = *iter;
}

} // jtx
} // test
} //
