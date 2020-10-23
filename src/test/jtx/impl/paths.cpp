//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/app/paths/Pathfinder.h>
#include <test/jtx/paths.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

void
paths::operator()(Env& env, JTx& jt) const
{
    auto& jv = jt.jv;
    auto const from = env.lookup(
        jv[jss::Account].asString());
    auto const to = env.lookup(
        jv[jss::Destination].asString());
    auto const amount = amountFromJson(
        sfAmount, jv[jss::Amount]);
    Pathfinder pf (
        std::make_shared<MTChainLineCache>(env.current()),
            from, to, in_.currency, in_.account,
                amount, boost::none, env.app());
    if (! pf.findPaths(depth_))
        return;

    STPath fp;
    pf.computePathRanks (limit_);
    auto const found = pf.getBestPaths(
        limit_, fp, {}, in_.account);

    // VFALCO TODO API to allow caller to examine the STPathSet
    // VFALCO isDefault should be renamed to empty()
    if (! found.isDefault())
        jv[jss::Paths] = found.getJson(0);
}

//------------------------------------------------------------------------------

Json::Value&
path::create()
{
    return jv_.append(Json::objectValue);
}

void
path::append_one(Account const& account)
{
    auto& jv = create();
    jv["account"] = toBase58(account.id());
}

void
path::append_one(IOU const& iou)
{
    auto& jv = create();
    jv["currency"] = to_string(iou.issue().currency);
    jv["account"] = toBase58(iou.issue().account);
}

void
path::append_one(BookSpec const& book)
{
    auto& jv = create();
    jv["currency"] = to_string(book.currency);
    jv["issuer"] = toBase58(book.account);
}

void
path::operator()(Env& env, JTx& jt) const
{
    jt.jv["Paths"].append(jv_);
}

} // jtx
} // test
} //
