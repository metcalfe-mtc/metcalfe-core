//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_PROP_H_INCLUDED
#define MTCHAIN_TEST_JTX_PROP_H_INCLUDED

#include <test/jtx/Env.h>
#include <memory>

namespace mtchain {
namespace test {
namespace jtx {

/** Set a property on a JTx. */
template <class Prop>
struct prop
{
    std::unique_ptr<basic_prop> p_;

    template <class... Args>
    prop(Args&&... args)
        : p_(std::make_unique<
            prop_type<Prop>>(
                std::forward <Args> (
                    args)...))
    {
    }

    void
    operator()(Env& env, JTx& jt) const
    {
        jt.set(p_->clone());
    }
};

} // jtx
} // test
} //

#endif
