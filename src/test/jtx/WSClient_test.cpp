//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/WSClient.h>
#include <test/jtx.h>
#include <mtchain/beast/unit_test.h>
#include <beast/core/handler_alloc.hpp>

namespace mtchain {
namespace test {

class WSClient_test : public beast::unit_test::suite
{
public:
    void run() override
    {
        using namespace jtx;
        Env env(*this);
        auto wsc = makeWSClient(env.app().config());
        {
            Json::Value jv;
            jv["streams"] = Json::arrayValue;
            jv["streams"].append("ledger");
        }
        env.fund(M(10000), "alice");
        env.close();
        auto jv = wsc->getMsg(std::chrono::seconds(1));
        pass();
    }
};

BEAST_DEFINE_TESTSUITE(WSClient,test,mtchain);

} // test
} //
