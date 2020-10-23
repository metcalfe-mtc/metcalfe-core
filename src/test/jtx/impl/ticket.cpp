//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/ticket.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {
namespace jtx {

namespace ticket {

namespace detail {

Json::Value
create (Account const& account,
    boost::optional<Account> const& target,
        boost::optional<std::uint32_t> const& expire)
{
    Json::Value jv;
    jv[jss::Account] = account.human();
    jv[jss::TransactionType] = "TicketCreate";
    if (expire)
        jv["Expiration"] = *expire;
    if (target)
        jv["Target"] = target->human();
    return jv;
}

} // detail

Json::Value
cancel(Account const& account, std::string const & ticketId)
{
    Json::Value jv;
    jv[jss::TransactionType] = "TicketCancel";
    jv[jss::Account] = account.human();
    jv["TicketID"] = ticketId;
    return jv;
}

} // ticket

} // jtx
} // test
} //
