//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_TAG_H_INCLUDED
#define MTCHAIN_TEST_JTX_TAG_H_INCLUDED

#include <test/jtx/Env.h>

namespace mtchain {
namespace test {

namespace jtx {

/** Set the destination tag on a JTx*/
struct dtag
{
private:
    std::uint32_t value_;

public:
    explicit
    dtag (std::uint32_t value)
        : value_ (value)
    {
    }

    void
    operator()(Env&, JTx& jt) const;
};

/** Set the source tag on a JTx*/
struct stag
{
private:
    std::uint32_t value_;

public:
    explicit
    stag (std::uint32_t value)
        : value_ (value)
    {
    }

    void
    operator()(Env&, JTx& jt) const;
};

} // jtx

} // test
} //

#endif
