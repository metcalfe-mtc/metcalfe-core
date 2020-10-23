//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <mtchain/protocol/JsonFields.h>
#include <test/jtx.h>

namespace mtchain {

namespace test {

class NoMTChain_test : public beast::unit_test::suite
{
public:
    void
    testSetAndClear()
    {
        testcase("Set and clear nomtchain");

        using namespace jtx;
        Env env(*this);

        auto const gw = Account("gateway");
        auto const alice = Account("alice");

        env.fund(M(10000), gw, alice);

        auto const USD = gw["USD"];

        Json::Value account_gw;
        account_gw[jss::account] = gw.human();
        Json::Value account_alice;
        account_alice[jss::account] = alice.human();

        for (auto SetOrClear : {true,false})
        {
            // Create a trust line with no-mtchain flag setting
            env( trust(gw, USD(100), alice, SetOrClear ? tfSetNoMtchain
                                                       : tfClearNoMtchain));
            env.close();

            // Check no-mtchain flag on sender 'gateway'
            auto lines = env.rpc("json", "account_lines", to_string(account_gw));
            auto const& gline0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(gline0[jss::no_mtchain].asBool() == SetOrClear);

            // Check no-mtchain peer flag on destination 'alice'
            lines = env.rpc("json", "account_lines", to_string(account_alice));
            auto const& aline0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(aline0[jss::no_mtchain_peer].asBool() == SetOrClear);
        }
    }

    void
    testNegativeBalance()
    {
        testcase("Set nomtchain on a line with negative balance");

        using namespace jtx;
        Env env(*this);

        auto const gw = Account("gateway");
        auto const alice = Account("alice");
        auto const bob = Account("bob");
        auto const carol = Account("carol");

        env.fund(M(10000), gw, alice, bob, carol);

        env.trust(alice["USD"](100), bob);
        env.trust(bob["USD"](100), carol);
        env.close();

        env(pay(alice, carol, carol["USD"](50)), path(bob));

        env(trust(alice, bob["USD"](100), bob, tfSetNoMtchain));
        env(trust(bob, carol["USD"](100), carol, tfSetNoMtchain));
        env.close();

        Json::Value params;
        params[jss::source_account] = alice.human();
        params[jss::destination_account] = carol.human();
        params[jss::destination_amount] = [] {
            Json::Value dest_amt;
            dest_amt[jss::currency] = "USD";
            dest_amt[jss::value] = "1";
            dest_amt[jss::issuer] = Account("carol").human();
            return dest_amt;
        }();

        auto const resp = env.rpc("json", "mtchain_path_find", to_string(params));
        BEAST_EXPECT(resp[jss::result][jss::alternatives].size()==1);

        Json::Value account_alice;
        account_alice[jss::account] = alice.human();
        auto const res = env.rpc("json", "account_lines", to_string(account_alice));
        auto const& lines = res[jss::result][jss::lines];
        BEAST_EXPECT(lines.size() == 1);
        BEAST_EXPECT(!lines[0u].isMember(jss::no_mtchain));
    }

    void
    testPairwise()
    {
        testcase("pairwise NoMTChain");

        using namespace jtx;
        Env env(*this);

        auto const alice = Account("alice");
        auto const bob = Account("bob");
        auto const carol = Account("carol");

        env.fund(M(10000), alice, bob, carol);

        env(trust(bob, alice["USD"](100)));
        env(trust(carol, bob["USD"](100)));

        env(trust(bob, alice["USD"](100), alice, tfSetNoMtchain));
        env(trust(bob, carol["USD"](100), carol, tfSetNoMtchain));
        env.close();

        Json::Value params;
        params[jss::source_account] = alice.human();
        params[jss::destination_account] = carol.human();
        params[jss::destination_amount] = [] {
            Json::Value dest_amt;
            dest_amt[jss::currency] = "USD";
            dest_amt[jss::value] = "1";
            dest_amt[jss::issuer] = Account("carol").human();
            return dest_amt;
        }();

        auto const resp = env.rpc("json", "mtchain_path_find", to_string(params));
        BEAST_EXPECT(resp[jss::result][jss::alternatives].size() == 0);

        env(pay(alice, carol, bob["USD"](50)), ter(tecPATH_DRY));
    }

    void
    testDefaultMTChain()
    {
        testcase("Set default mtchain on an account and check new trustlines");

        using namespace jtx;
        Env env(*this);

        auto const gw = Account("gateway");
        auto const alice = Account("alice");
        auto const bob =   Account("bob");

        env.fund(M(10000), gw, nomtchain(alice, bob));

        env(fset(bob, asfDefaultMtchain));

        auto const USD = gw["USD"];

        env(trust(gw, USD(100), alice, 0));
        env(trust(gw, USD(100), bob, 0));

        {
            Json::Value params;
            params[jss::account] = gw.human();
            params[jss::peer] = alice.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_mtchain_peer].asBool() == true);
        }
        {
            Json::Value params;
            params[jss::account] = alice.human();
            params[jss::peer] = gw.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_mtchain].asBool() == true);
        }
        {
            Json::Value params;
            params[jss::account] = gw.human();
            params[jss::peer] = bob.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_mtchain].asBool() == false);
        }
        {
            Json::Value params;
            params[jss::account] = bob.human();
            params[jss::peer] = gw.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_mtchain_peer].asBool() == false);
        }
    }

    void run ()
    {
        testSetAndClear();
        testNegativeBalance();
        testPairwise();
        testDefaultMTChain();
    }
};

BEAST_DEFINE_TESTSUITE(NoMTChain,app,mtchain);

} // RPC
} //

