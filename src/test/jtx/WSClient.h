//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_WSCLIENT_H_INCLUDED
#define MTCHAIN_TEST_WSCLIENT_H_INCLUDED

#include <test/jtx/AbstractClient.h>
#include <mtchain/core/Config.h>
#include <boost/optional.hpp>
#include <chrono>
#include <memory>

namespace mtchain {
namespace test {

class WSClient : public AbstractClient
{
public:
    /** Retrieve a message. */
    virtual
    boost::optional<Json::Value>
    getMsg(std::chrono::milliseconds const& timeout =
        std::chrono::milliseconds{0}) = 0;

    /** Retrieve a message that meets the predicate criteria. */
    virtual
    boost::optional<Json::Value>
    findMsg(std::chrono::milliseconds const& timeout,
        std::function<bool(Json::Value const&)> pred) = 0;
};

/** Returns a client operating through WebSockets/S. */
std::unique_ptr<WSClient>
makeWSClient(Config const& cfg, bool v2 = true, unsigned rpc_version = 2);

} // test
} //

#endif
