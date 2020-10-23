//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/quality.h>
#include <mtchain/protocol/SField.h>
#include <mtchain/protocol/Quality.h>

namespace mtchain {
namespace test {
namespace jtx {

qualityInPercent::qualityInPercent (double percent)
: qIn_ (static_cast<std::uint32_t>((percent / 100) * QUALITY_ONE))
{
    assert (percent <= 400 && percent >= 0);
}

qualityOutPercent::qualityOutPercent (double percent)
: qOut_ (static_cast<std::uint32_t>((percent / 100) * QUALITY_ONE))
{
    assert (percent <= 400 && percent >= 0);
}

static void
insertQualityIntoJtx (SField const& field, std::uint32_t value, JTx& jt)
{
    jt.jv[field.jsonName] = value;
}

void
qualityIn::operator()(Env&, JTx& jt) const
{
    insertQualityIntoJtx (sfQualityIn, qIn_, jt);
}

void
qualityInPercent::operator()(Env&, JTx& jt) const
{
    insertQualityIntoJtx (sfQualityIn, qIn_, jt);
}

void
qualityOut::operator()(Env&, JTx& jt) const
{
    insertQualityIntoJtx (sfQualityOut, qOut_, jt);
}

void
qualityOutPercent::operator()(Env&, JTx& jt) const
{
    insertQualityIntoJtx (sfQualityOut, qOut_, jt);
}

} // jtx
} // test
} //
