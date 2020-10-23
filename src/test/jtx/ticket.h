//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_TICKET_H_INCLUDED
#define MTCHAIN_TEST_JTX_TICKET_H_INCLUDED

#include <test/jtx/Env.h>
#include <test/jtx/Account.h>
#include <test/jtx/owners.h>
#include <boost/optional.hpp>
#include <cstdint>

namespace mtchain {
namespace test {
namespace jtx {

/*
    This shows how the jtx system may be extended to other
    generators, funclets, conditions, and operations,
    without changing the base declarations.
*/

/** Ticket operations */
namespace ticket {

namespace detail {

Json::Value
create (Account const& account,
    boost::optional<Account> const& target,
        boost::optional<std::uint32_t> const& expire);

inline
void
create_arg (boost::optional<Account>& opt,
    boost::optional<std::uint32_t>&,
        Account const& value)
{
    opt = value;
}

inline
void
create_arg (boost::optional<Account>&,
    boost::optional<std::uint32_t>& opt,
        std::uint32_t value)
{
    opt = value;
}

inline
void
create_args (boost::optional<Account>&,
    boost::optional<std::uint32_t>&)
{
}

template<class Arg, class... Args>
void
create_args(boost::optional<Account>& account_opt,
    boost::optional<std::uint32_t>& expire_opt,
        Arg const& arg, Args const&... args)
{
    create_arg(account_opt, expire_opt, arg);
    create_args(account_opt, expire_opt, args...);
}

} // detail

/** Create a ticket */
template <class... Args>
Json::Value
create (Account const& account,
    Args const&... args)
{
    boost::optional<Account> target;
    boost::optional<std::uint32_t> expire;
    detail::create_args(target, expire, args...);
    return detail::create(
        account, target, expire);
}

/** Cancel a ticket */
Json::Value
cancel(Account const& account, std::string const & ticketId);

} // ticket

/** Match the number of tickets on the account. */
using tickets = owner_count<ltTICKET>;

} // jtx

} // test
} //

#endif
