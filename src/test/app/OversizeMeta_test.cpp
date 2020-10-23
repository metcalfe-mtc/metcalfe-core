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
#include <algorithm>

namespace mtchain {
namespace test {

// Make sure "plump" order books don't have problems
class PlumpBook_test : public beast::unit_test::suite
{
public:
    void
    createOffers (jtx::Env& env,
        jtx::IOU const& iou, std::size_t n)
    {
        using namespace jtx;
        for (std::size_t i = 1; i <= n; ++i)
            env(offer("alice", M(i), iou(1)));
    }

    void
    test (std::size_t n)
    {
        using namespace jtx;
        auto const billion = 1000000000ul;
        Env env(*this);
        env.disable_sigs();
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];
        env.fund(M(billion), gw, "alice", "bob", "carol");
        env.trust(USD(billion), "alice", "bob", "carol");
        env(pay(gw, "alice", USD(billion)));
        createOffers(env, USD, n);
    }

    void
    run() override
    {
        test(10000);
    }
};

BEAST_DEFINE_TESTSUITE_MANUAL(PlumpBook,tx,mtchain);

//------------------------------------------------------------------------------

// Ensure that unsigned transactions succeed during automatic test runs.
class ThinBook_test : public PlumpBook_test
{
public:
    void
        run() override
    {
        test(1);
    }
};

BEAST_DEFINE_TESTSUITE(ThinBook, tx, mtchain);

//------------------------------------------------------------------------------

class OversizeMeta_test : public beast::unit_test::suite
{
public:
    void
    createOffers (jtx::Env& env, jtx::IOU const& iou,
        std::size_t n)
    {
        using namespace jtx;
        for (std::size_t i = 1; i <= n; ++i)
            env(offer("alice", M(1), iou(1)));
    }

    void
    test()
    {
        std::size_t const n = 9000;
        using namespace jtx;
        auto const billion = 1000000000ul;
        Env env(*this);
        env.disable_sigs();
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];
        env.fund(M(billion), gw, "alice", "bob", "carol");
        env.trust(USD(billion), "alice", "bob", "carol");
        env(pay(gw, "alice", USD(billion)));
        createOffers(env, USD, n);
        env(pay("alice", gw, USD(billion)));
        env(offer("alice", USD(1), M(1)));
    }

    void
    run()
    {
        test();
    }
};

BEAST_DEFINE_TESTSUITE_MANUAL(OversizeMeta,tx,mtchain);

//------------------------------------------------------------------------------

class FindOversizeCross_test : public beast::unit_test::suite
{
public:
    // Return lowest x in [lo, hi] for which f(x)==true
    template <class Function>
    static
    std::size_t
    bfind(std::size_t lo, std::size_t hi, Function&& f)
    {
        auto len = hi - lo;
        while (len != 0)
        {
            auto l2 = len / 2;
            auto m = lo + l2;
            if (! f(m))
            {
                lo = ++m;
                len -= l2 + 1;
            }
            else
                len = l2;
        }
        return lo;
    }

    void
    createOffers (jtx::Env& env, jtx::IOU const& iou,
        std::size_t n)
    {
        using namespace jtx;
        for (std::size_t i = 1; i <= n; ++i)
            env(offer("alice", M(i), iou(1)));
    }

    bool
    oversize(std::size_t n)
    {
        using namespace jtx;
        auto const billion = 1000000000ul;
        Env env(*this);
        env.disable_sigs();
        auto const gw = Account("gateway");
        auto const USD = gw["USD"];
        env.fund(M(billion), gw, "alice", "bob", "carol");
        env.trust(USD(billion), "alice", "bob", "carol");
        env(pay(gw, "alice", USD(billion)));
        createOffers(env, USD, n);
        env(pay("alice", gw, USD(billion)));
        env(offer("alice", USD(1), M(1)), ter(std::ignore));
        return env.ter() == tecOVERSIZE;
    }

    void
    run()
    {
        auto const result = bfind(100, 9000,
            [&](std::size_t n) { return oversize(n); });
        log << "Min oversize offers = " << result << '\n';
    }
};

BEAST_DEFINE_TESTSUITE_MANUAL(FindOversizeCross,tx,mtchain);

} // test
} //

