//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_UTILITY_H_INCLUDED
#define MTCHAIN_TEST_JTX_UTILITY_H_INCLUDED

#include <test/jtx/Account.h>
#include <mtchain/json/json_value.h>
#include <mtchain/app/ledger/Ledger.h>
#include <mtchain/protocol/STObject.h>
#include <stdexcept>

namespace mtchain {
namespace test {
namespace jtx {

/** Thrown when parse fails. */
struct parse_error : std::logic_error
{
    template <class String>
    explicit
    parse_error (String const& s)
        : logic_error(s)
    {
    }
};

/** Convert JSON to STObject.
    This throws on failure, the JSON must be correct.
    @note Testing malformed JSON is beyond the scope of
          this set of unit test routines.
*/
STObject
parse (Json::Value const& jv);

/** Sign automatically.
    @note This only works on accounts with multi-signing off.
*/
void
sign (Json::Value& jv,
    Account const& account);

/** Set the fee automatically. */
void
fill_fee (Json::Value& jv,
    ReadView const& view);

/** Set the sequence number automatically. */
void
fill_seq (Json::Value& jv,
    ReadView const& view);

} // jtx
} // test
} //

#endif
