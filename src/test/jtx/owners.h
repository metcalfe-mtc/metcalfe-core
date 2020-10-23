//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_OWNERS_H_INCLUDED
#define MTCHAIN_TEST_JTX_OWNERS_H_INCLUDED

#include <test/jtx/Env.h>
#include <mtchain/ledger/View.h>
#include <mtchain/protocol/LedgerFormats.h>
#include <mtchain/protocol/UintTypes.h>
#include <cstdint>

namespace mtchain {
namespace test {
namespace jtx {

namespace detail {

std::uint32_t
owned_count_of (ReadView const& view,
    AccountID const& id,
        LedgerEntryType type);

void
owned_count_helper(Env& env,
    AccountID const& id,
        LedgerEntryType type,
            std::uint32_t value);

} // detail

// Helper for aliases
template <LedgerEntryType Type>
class owner_count
{
private:
    Account account_;
    std::uint32_t value_;

public:
    owner_count (Account const& account,
            std::uint32_t value)
        : account_(account)
        , value_(value)
    {
    }

    void
    operator()(Env& env) const
    {
        detail::owned_count_helper(
            env, account_.id(), Type, value_);
    }
};

/** Match the number of items in the account's owner directory */
class owners
{
private:
    Account account_;
    std::uint32_t value_;
public:
    owners (Account const& account,
            std::uint32_t value)
        : account_(account)
        , value_(value)
    {
    }

    void
    operator()(Env& env) const;
};

/** Match the number of trust lines in the account's owner directory */
using lines = owner_count<ltMTCHAIN_STATE>;

/** Match the number of offers in the account's owner directory */
using offers = owner_count<ltOFFER>;

} // jtx
} // test
} //

#endif
