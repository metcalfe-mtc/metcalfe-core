//------------------------------------------------------------------------------
/*
  This file is part of mtchaind: https://github.com/MTChain/MTChain-core
  Copyright (c) 2012-2015 MTChain Alliance.

  Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {
namespace test {

class DeliverMin_test : public beast::unit_test::suite
{
public:
    void
    test_convert_all_of_an_asset()
    {
        testcase("Convert all of an asset using DeliverMin");

        using namespace jtx;
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];

        {
            Env env(*this);
            env.fund(M(10000), "alice", "bob", "carol", gw);
            env.trust(USD(100), "alice", "bob", "carol");
            env(pay("alice", "bob", USD(10)), delivermin(USD(10)),  ter(temBAD_AMOUNT));
            env(pay("alice", "bob", USD(10)), delivermin(USD(-5)),
                txflags(tfPartialPayment),                          ter(temBAD_AMOUNT));
            env(pay("alice", "bob", USD(10)), delivermin(M(5)),
                txflags(tfPartialPayment),                          ter(temBAD_AMOUNT));
            env(pay("alice", "bob", USD(10)),
                delivermin(Account("carol")["USD"](5)),
                txflags(tfPartialPayment),                          ter(temBAD_AMOUNT));
            env(pay("alice", "bob", USD(10)), delivermin(USD(15)),
                txflags(tfPartialPayment),                          ter(temBAD_AMOUNT));
            env(pay(gw, "carol", USD(50)));
            env(offer("carol", M(5), USD(5)));
            env(pay("alice", "bob", USD(10)), paths(M),
                delivermin(USD(7)), txflags(tfPartialPayment),
                sendmax(M(5)),                                   ter(tecPATH_PARTIAL));
            env.require(balance("alice", M(9999.99999)));
            env.require(balance("bob", M(10000)));
        }

        {
            Env env(*this);
            env.fund(M(10000), "alice", "bob", gw);
            env.trust(USD(1000), "alice", "bob");
            env(pay(gw, "bob", USD(100)));
            env(offer("bob", M(100), USD(100)));
            env(pay("alice", "alice", USD(10000)), paths(M),
                delivermin(USD(100)), txflags(tfPartialPayment),
                sendmax(M(100)));
            env.require(balance("alice", USD(100)));
        }

        {
            Env env(*this);
            env.fund(M(10000), "alice", "bob", "carol", gw);
            env.trust(USD(1000), "bob", "carol");
            env(pay(gw, "bob", USD(200)));
            env(offer("bob", M(100), USD(100)));
            env(offer("bob", M(1000), USD(100)));
            env(offer("bob", M(10000), USD(100)));
            env(pay("alice", "carol", USD(10000)), paths(M),
                delivermin(USD(200)), txflags(tfPartialPayment),
                sendmax(M(1000)),                                 ter(tecPATH_PARTIAL));
            env(pay("alice", "carol", USD(10000)), paths(M),
                delivermin(USD(200)), txflags(tfPartialPayment),
                sendmax(M(1100)));
            env.require(balance("bob", USD(0)));
            env.require(balance("carol", USD(200)));
        }

        {
            Env env(*this);
            env.fund(M(10000), "alice", "bob", "carol", "dan", gw);
            env.trust(USD(1000), "bob", "carol", "dan");
            env(pay(gw, "bob", USD(100)));
            env(pay(gw, "dan", USD(100)));
            env(offer("bob", M(100), USD(100)));
            env(offer("bob", M(1000), USD(100)));
            env(offer("dan", M(100), USD(100)));
            env(pay("alice", "carol", USD(10000)), paths(M),
                delivermin(USD(200)), txflags(tfPartialPayment),
                sendmax(M(200)));
            env.require(balance("bob", USD(0)));
            env.require(balance("carol", USD(200)));
            env.require(balance("dan", USD(0)));
        }
    }

    void
    run()
    {
        test_convert_all_of_an_asset();
    }
};

BEAST_DEFINE_TESTSUITE(DeliverMin,app,mtchain);

} // test
} //
