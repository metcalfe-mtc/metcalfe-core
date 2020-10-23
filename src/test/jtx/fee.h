//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_FEE_H_INCLUDED
#define MTCHAIN_TEST_JTX_FEE_H_INCLUDED

#include <test/jtx/Env.h>
#include <test/jtx/tags.h>
#include <mtchain/protocol/STAmount.h>
#include <mtchain/basics/contract.h>
#include <boost/optional.hpp>

namespace mtchain {
namespace test {
namespace jtx {

/** Set the fee on a JTx. */
class fee
{
private:
    bool manual_ = true;
    boost::optional<STAmount> amount_;

public:
    explicit
    fee (autofill_t)
        : manual_(false)
    {
    }

    explicit
    fee (none_t)
    {
    }

    explicit
    fee (STAmount const& amount)
        : amount_(amount)
    {
        if (! isM(*amount_))
            Throw<std::runtime_error> (
                "fee: not M");
    }

    void
    operator()(Env&, JTx& jt) const;
};

} // jtx
} // test
} //

#endif
