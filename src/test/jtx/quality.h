//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_QUALITY_H_INCLUDED
#define MTCHAIN_TEST_JTX_QUALITY_H_INCLUDED

#include <test/jtx/Env.h>

namespace mtchain {
namespace test {
namespace jtx {

/** Sets the literal QualityIn on a trust JTx. */
class qualityIn
{
private:
    std::uint32_t qIn_;

public:
    explicit qualityIn (std::uint32_t qIn)
    : qIn_ (qIn)
    {
    }

    void
    operator()(Env&, JTx& jtx) const;
};

/** Sets the QualityIn on a trust JTx as a percentage. */
class qualityInPercent
{
private:
    std::uint32_t qIn_;

public:
    explicit qualityInPercent (double percent);

    void
    operator()(Env&, JTx& jtx) const;
};

/** Sets the literal QualityOut on a trust JTx. */
class qualityOut
{
private:
    std::uint32_t qOut_;

public:
    explicit qualityOut (std::uint32_t qOut)
    : qOut_ (qOut)
    {
    }

    void
    operator()(Env&, JTx& jtx) const;
};

/** Sets the QualityOut on a trust JTx as a percentage. */
class qualityOutPercent
{
private:
    std::uint32_t qOut_;

public:
    explicit qualityOutPercent (double percent);

    void
    operator()(Env&, JTx& jtx) const;
};

} // jtx
} // test
} //

#endif
