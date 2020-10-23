//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_SIG_H_INCLUDED
#define MTCHAIN_TEST_JTX_SIG_H_INCLUDED

#include <test/jtx/Env.h>
#include <boost/optional.hpp>

namespace mtchain {
namespace test {
namespace jtx {

/** Set the regular signature on a JTx.
    @note For multisign, use msig.
*/
class sig
{
private:
    bool manual_ = true;
    boost::optional<Account> account_;

public:
    explicit
    sig (autofill_t)
        : manual_(false)
    {
    }

    explicit
    sig (none_t)
    {
    }

    explicit
    sig (Account const& account)
        : account_(account)
    {
    }

    void
    operator()(Env&, JTx& jt) const;
};

} // jtx
} // test
} //

#endif
