//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_ABSTRACTCLIENT_H_INCLUDED
#define MTCHAIN_TEST_ABSTRACTCLIENT_H_INCLUDED

#include <mtchain/json/json_value.h>

namespace mtchain {
namespace test {

/* Abstract MTChain Client interface.

   This abstracts the transport layer, allowing
   commands to be submitted to a mtchaind server.
*/
class AbstractClient
{
public:
    virtual ~AbstractClient() = default;
    AbstractClient() = default;
    AbstractClient(AbstractClient const&) = delete;
    AbstractClient& operator=(AbstractClient const&) = delete;

    /** Submit a command synchronously.

        The arguments to the function and the returned JSON
        are in a normalized format, the same whether the client
        is using the JSON-RPC over HTTP/S or WebSocket transport.

        @param cmd The command to execute
        @param params Json::Value of null or object type
                      with zero or more key/value pairs.
        @return The server response in normalized format.
    */
    virtual
    Json::Value
    invoke(std::string const& cmd,
        Json::Value const& params = {}) = 0;

    /// Get RPC 1.0 or RPC 2.0
    virtual unsigned version() const = 0;
};

} // test
} //

#endif
