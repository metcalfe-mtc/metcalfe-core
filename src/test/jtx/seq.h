//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_SEQ_H_INCLUDED
#define MTCHAIN_TEST_JTX_SEQ_H_INCLUDED

#include <test/jtx/Env.h>
#include <test/jtx/tags.h>
#include <boost/optional.hpp>

namespace mtchain {
namespace test {
namespace jtx {

/** Set the sequence number on a JTx. */
struct seq
{
private:
    bool manual_ = true;
    boost::optional<std::uint32_t> num_;

public:
    explicit
    seq (autofill_t)
        : manual_(false)
    {
    }

    explicit
    seq (none_t)
    {
    }

    explicit
    seq (std::uint32_t num)
        : num_(num)
    {
    }

    void
    operator()(Env&, JTx& jt) const;
};

} // jtx
} // test
} //

#endif
