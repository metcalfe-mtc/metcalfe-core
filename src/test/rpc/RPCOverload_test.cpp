//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.
    Permission to use, copy, modify, and/or distribute this software for any
*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/protocol/JsonFields.h>
#include <test/jtx/WSClient.h>
#include <test/jtx/JSONRPCClient.h>
#include <test/jtx.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {
namespace test {

class RPCOverload_test : public beast::unit_test::suite
{
public:
    void testOverload(bool useWS)
    {
        testcase << "Overload " << (useWS ? "WS" : "HTTP") << " RPC client";
        using namespace jtx;
        Env env(*this, []()
            {
                auto p = std::make_unique<Config>();
                setupConfigForUnitTests(*p);
                (*p)["port_rpc"].set("admin","");
                (*p)["port_ws"].set("admin","");
                return p;
            }());

        Account const alice {"alice"};
        Account const bob {"bob"};
        env.fund (M (10000), alice, bob);

        std::unique_ptr<AbstractClient> client = useWS ?
              makeWSClient(env.app().config())
            : makeJSONRPCClient(env.app().config());

        Json::Value tx = Json::objectValue;
        tx[jss::tx_json] = pay(alice, bob, M(1));
        tx[jss::secret] = toBase58(generateSeed("alice"));

        // Ask the server to repeatedly sign this transaction
        // Signing is a resource heavy transaction, so we want the server
        // to warn and eventually boot us.
        bool warned = false, booted = false;
        for(int i = 0 ; i < 500 && !booted; ++i)
        {
            auto jv = client->invoke("sign", tx);
            if(!useWS)
                jv = jv[jss::result];
            // When booted, we just get a null json response
            if(jv.isNull())
                booted = true;
            else
                BEAST_EXPECT(jv.isMember(jss::status)
                             && (jv[jss::status] == "success"));

            if(jv.isMember(jss::warning))
                warned = jv[jss::warning] == jss::load;
        }
        BEAST_EXPECT(warned && booted);
    }



    void run() override
    {
        testOverload(false /* http */);
        testOverload(true /* ws */);
    }
};

BEAST_DEFINE_TESTSUITE(RPCOverload,app,mtchain);

} // test
} //
