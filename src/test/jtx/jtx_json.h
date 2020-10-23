//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_JSON_H_INCLUDED
#define MTCHAIN_TEST_JTX_JSON_H_INCLUDED

#include <test/jtx/Env.h>
#include <mtchain/json/json_value.h>

namespace mtchain {
namespace test {
namespace jtx {

/** Inject raw JSON. */
class json
{
private:
    Json::Value jv_;

public:
    explicit
    json (std::string const&);

    explicit
    json (char const*);

    explicit
    json (Json::Value);

    template <class T>
    json (Json::StaticString const& key, T const& value)
    {
        jv_[key] = value;
    }

    template <class T>
    json (std::string const& key, T const& value)
    {
        jv_[key] = value;
    }

    void
    operator()(Env&, JTx& jt) const;
};

} // jtx
} // test
} //

#endif
