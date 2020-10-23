//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.
    Permission to use, copy, modify, and/or distribute this software for any
*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {
namespace test {

class CrossingLimits_test : public beast::unit_test::suite
{
private:
    void
    n_offers (
        jtx::Env& env,
        std::size_t n,
        jtx::Account const& account,
        STAmount const& in,
        STAmount const& out)
    {
        using namespace jtx;
        auto const ownerCount = env.le(account)->getFieldU32(sfOwnerCount);
        for (std::size_t i = 0; i < n; i++)
            env(offer(account, in, out));
        env.require (owners (account, ownerCount + n));
    }

public:
    void
    testStepLimit()
    {
        using namespace jtx;
        Env env(*this);
        auto const mMax = M(100000000000);
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];

        env.fund(M(100000000), gw, "alice", "bob", "carol", "dan");
        env.trust(USD(1), "bob");
        env(pay(gw, "bob", USD(1)));
        env.trust(USD(1), "dan");
        env(pay(gw, "dan", USD(1)));
        n_offers (env, 2000, "bob", M(1), USD(1));
        n_offers (env, 1, "dan", M(1), USD(1));

        // Alice offers to buy 1000 M for 1000 USD. She takes Bob's first
        // offer, and removes 999 more as unfunded and hits the step limit.
        env(offer("alice", USD(1000), M(1000)),
            require (
                balance("alice", USD(1)), owners("alice", 2),
                balance("bob", USD(0)), owners("bob", 1001),
                balance("dan", USD(1)), owners("dan", 2)));

        // Carol offers to buy 1000 M for 1000 USD. She removes Bob's next
        // 1000 offers as unfunded and hits the step limit.
        env(offer("carol", USD(1000), M(1000)),
            require (
                balance("carol", USD(none)), owners("carol", 1),
                balance("bob", USD(0)), owners("bob", 1),
                balance("dan", USD(1)), owners("dan", 2)));
    }

    void
    testCrossingLimit()
    {
        using namespace jtx;
        Env env(*this);
        auto const mMax = M(100000000000);
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];

        env.fund(M(100000000), gw, "alice", "bob", "carol");
        env.trust(USD(1000), "bob");
        env(pay(gw, "bob", USD(1000)));
        n_offers (env, 1000, "bob", M(1), USD(1));

        // Alice offers to buy 1000 M for 1000 USD. She takes the first
        // 850 offers, hitting the crossing limit.
        env(offer("alice", USD(1000), M(1000)),
            require (
                balance("alice", USD(850)),
                balance("bob", USD(150)), owners ("bob", 151)));

        // Carol offers to buy 1000 M for 1000 USD. She takes the remaining
        // 150 offers without hitting a limit.
        env(offer("carol", USD(1000), M(1000)),
            require (
                balance("carol", USD(150)),
                balance("bob", USD(0)), owners ("bob", 1)));
    }

    void
    testStepAndCrossingLimit()
    {
        using namespace jtx;
        Env env(*this);
        auto const mMax = M(100000000000);
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];

        env.fund(M(100000000), gw, "alice", "bob", "carol", "dan", "evita");

        env.trust(USD(1000), "alice");
        env(pay(gw, "alice", USD(1000)));
        env.trust(USD(1000), "carol");
        env(pay(gw, "carol", USD(1)));
        env.trust(USD(1000), "evita");
        env(pay(gw, "evita", USD(1000)));

        n_offers (env, 400, "alice", M(1), USD(1));
        n_offers (env, 700, "carol", M(1), USD(1));
        n_offers (env, 999, "evita", M(1), USD(1));

        // Bob offers to buy 1000 M for 1000 USD. He takes all 400 USD from
        // Alice's offers, 1 USD from Carol's and then removes 599 of Carol's
        // offers as unfunded, before hitting the step limit.
        env(offer("bob", USD(1000), M(1000)),
            require (
                balance("bob", USD(401)),
                balance("alice", USD(600)), owners("alice", 1),
                balance("carol", USD(0)), owners("carol", 101),
                balance("evita", USD(1000)), owners("evita", 1000)));

        // Dan offers to buy 900 M for 900 USD. He removes all 100 of Carol's
        // offers as unfunded, then takes 850 USD from Evita's, hitting the
        // crossing limit.
        env(offer("dan", USD(900), M(900)),
            require (
                balance("dan", USD(850)),
                balance("alice", USD(600)), owners("alice", 1),
                balance("carol", USD(0)), owners("carol", 1),
                balance("evita", USD(150)), owners("evita", 150)));
    }

    void
    run()
    {
        testStepLimit();
        testCrossingLimit();
        testStepAndCrossingLimit();
    }
};

BEAST_DEFINE_TESTSUITE_MANUAL(CrossingLimits,tx,mtchain);

} // test
} //
