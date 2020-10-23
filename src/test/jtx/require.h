//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_REQUIRE_H_INCLUDED
#define MTCHAIN_TEST_JTX_REQUIRE_H_INCLUDED

#include <test/jtx/requires.h>
#include <functional>
#include <vector>

namespace mtchain {
namespace test {
namespace jtx {

namespace detail {

inline
void
require_args (requires_t& vec)
{
}

template <class Cond, class... Args>
inline
void
require_args (requires_t& vec,
    Cond const& cond, Args const&... args)
{
    vec.push_back(cond);
    require_args(vec, args...);
}

} // detail

/** Compose many condition functors into one */
template <class...Args>
require_t
required (Args const&... args)
{
    requires_t vec;
    detail::require_args(vec, args...);
    return [vec](Env& env)
    {
        for(auto const& f : vec)
            f(env);
    };
}

/** Check a set of conditions.

    The conditions are checked after a JTx is
    applied, and only if the resulting TER
    matches the expected TER.
*/
class require
{
private:
    require_t cond_;

public:
    template<class... Args>
    require(Args const&... args)
        : cond_(required(args...))
    {
    }

    void
    operator()(Env&, JTx& jt) const
    {
        jt.requires.emplace_back(cond_);
    }
};

} // jtx
} // test
} //

#endif
