//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx.h>
#include <mtchain/protocol/Feature.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {
namespace test {

struct SetAuth_test : public beast::unit_test::suite
{
    // Set just the tfSetfAuth flag on a trust line
    // If the trust line does not exist, then it should
    // be created under the new rules.
    static
    Json::Value
    auth (jtx::Account const& account,
        jtx::Account const& dest,
            std::string const& currency)
    {
        using namespace jtx;
        Json::Value jv;
        jv[jss::Account] = account.human();
        jv[jss::LimitAmount] = STAmount(
            { to_currency(currency), dest }).getJson(0);
        jv[jss::TransactionType] = "TrustSet";
        jv[jss::Flags] = tfSetfAuth;
        return jv;
    }

    void testAuth()
    {
        using namespace jtx;
        auto const gw = Account("gw");
        auto const USD = gw["USD"];
        {
            Env env(*this);
            env.fund(M(100000), "alice", gw);
            env(fset(gw, asfRequireAuth));
            env(auth(gw, "alice", "USD"),       ter(tecNO_LINE_REDUNDANT));
        }
        {
            Env env(*this, features(featureTrustSetAuth));
            env.fund(M(100000), "alice", "bob", gw);
            env(fset(gw, asfRequireAuth));
            env(auth(gw, "alice", "USD"));
            BEAST_EXPECT(env.le(
                keylet::line(Account("alice").id(),
                    gw.id(), USD.currency)));
            env(trust("alice", USD(1000)));
            env(trust("bob", USD(1000)));
            env(pay(gw, "alice", USD(100)));
            env(pay(gw, "bob", USD(100)),       ter(tecPATH_DRY)); // Should be terNO_AUTH
            env(pay("alice", "bob", USD(50)),   ter(tecPATH_DRY)); // Should be terNO_AUTH
        }
    }

    void run() override
    {
        testAuth();
    }
};

BEAST_DEFINE_TESTSUITE(SetAuth,test,mtchain);

} // test
} //
