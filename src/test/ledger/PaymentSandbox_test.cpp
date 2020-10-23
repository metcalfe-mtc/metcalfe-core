//------------------------------------------------------------------------------
/*
  This file is part of mtchaind: https://github.com/MTChain/MTChain-core
  Copyright (c) 2012-2015 MTChain Alliance.

  Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/ledger/ApplyViewImpl.h>
#include <mtchain/ledger/PaymentSandbox.h>
#include <test/jtx/PathSet.h>
#include <mtchain/ledger/View.h>
#include <mtchain/protocol/AmountConversions.h>

namespace mtchain {
namespace test {

class PaymentSandbox_test : public beast::unit_test::suite
{
    /*
      Create paths so one path funds another path.

      Two accounts: sender and receiver.
      Two gateways: gw1 and gw2.
      Sender and receiver both have trust lines to the gateways.
      Sender has 2 gw1/USD and 4 gw2/USD.
      Sender has offer to exchange 2 gw1 for gw2 and gw2 for gw1 1-for-1.
      Paths are:
      1) GW1 -> [OB GW1/USD->GW2/USD] -> GW2
      2) GW2 -> [OB GW2/USD->GW1/USD] -> GW1

      sender pays receiver 4 USD.
      Path 1:
      1) Sender exchanges 2 GW1/USD for 2 GW2/USD
      2) Old code: the 2 GW1/USD is available to sender
         New code: the 2 GW1/USD is not available until the
         end of the transaction.
      3) Receiver gets 2 GW2/USD
      Path 2:
      1) Old code: Sender exchanges 2 GW2/USD for 2 GW1/USD
      2) Old code: Receiver get 2 GW1
      2) New code: Path is dry because sender does not have any
         GW1 to spend until the end of the transaction.
    */
    void testSelfFunding ()
    {
        testcase ("selfFunding");

        using namespace jtx;
        Env env (*this);
        Account const gw1 ("gw1");
        Account const gw2 ("gw2");
        Account const snd ("snd");
        Account const rcv ("rcv");

        env.fund (M (10000), snd, rcv, gw1, gw2);

        auto const USD_gw1 = gw1["USD"];
        auto const USD_gw2 = gw2["USD"];

        env.trust (USD_gw1 (10), snd);
        env.trust (USD_gw2 (10), snd);
        env.trust (USD_gw1 (100), rcv);
        env.trust (USD_gw2 (100), rcv);

        env (pay (gw1, snd, USD_gw1 (2)));
        env (pay (gw2, snd, USD_gw2 (4)));

        env (offer (snd, USD_gw1 (2), USD_gw2 (2)),
             txflags (tfPassive));
        env (offer (snd, USD_gw2 (2), USD_gw1 (2)),
             txflags (tfPassive));

        PathSet paths (
            Path (gw1, USD_gw2, gw2),
            Path (gw2, USD_gw1, gw1));

        env (pay (snd, rcv, any (USD_gw1 (4))),
            json (paths.json ()),
            txflags (tfNoMTChainDirect | tfPartialPayment));

        env.require (balance ("rcv", USD_gw1 (0)));
        env.require (balance ("rcv", USD_gw2 (2)));
    }

    void testSubtractCredits ()
    {
        testcase ("subtractCredits");

        using namespace jtx;
        Env env (*this);
        Account const gw1 ("gw1");
        Account const gw2 ("gw2");
        Account const alice ("alice");

        env.fund (M (10000), alice, gw1, gw2);

        auto j = env.app().journal ("View");

        auto const USD_gw1 = gw1["USD"];
        auto const USD_gw2 = gw2["USD"];

        env.trust (USD_gw1 (100), alice);
        env.trust (USD_gw2 (100), alice);

        env (pay (gw1, alice, USD_gw1 (50)));
        env (pay (gw2, alice, USD_gw2 (50)));

        STAmount const toCredit (USD_gw1 (30));
        STAmount const toDebit (USD_gw1 (20));
        {
            // accountSend, no deferredCredits
            ApplyViewImpl av (&*env.current(), tapNONE);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                av, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            accountSend (av, gw1, alice, toCredit, j);
            BEAST_EXPECT(accountHolds (av, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount + toCredit);

            accountSend (av, alice, gw1, toDebit, j);
            BEAST_EXPECT(accountHolds (av, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount + toCredit - toDebit);
        }

        {
            // mtchainCredit, no deferredCredits
            ApplyViewImpl av (&*env.current(), tapNONE);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                av, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            mtchainCredit (av, gw1, alice, toCredit, true, j);
            BEAST_EXPECT(accountHolds (av, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount + toCredit);

            mtchainCredit (av, alice, gw1, toDebit, true, j);
            BEAST_EXPECT(accountHolds (av, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount + toCredit - toDebit);
        }

        {
            // accountSend, w/ deferredCredits
            ApplyViewImpl av (&*env.current(), tapNONE);
            PaymentSandbox pv (&av);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                pv, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            accountSend (pv, gw1, alice, toCredit, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount);

            accountSend (pv, alice, gw1, toDebit, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount - toDebit);
        }

        {
            // mtchainCredit, w/ deferredCredits
            ApplyViewImpl av (&*env.current(), tapNONE);
            PaymentSandbox pv (&av);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                pv, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            mtchainCredit (pv, gw1, alice, toCredit, true, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount);
        }

        {
            // redeemIOU, w/ deferredCredits
            ApplyViewImpl av (&*env.current(), tapNONE);
            PaymentSandbox pv (&av);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                pv, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            redeemIOU (pv, alice, toDebit, iss, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount - toDebit);
        }

        {
            // issueIOU, w/ deferredCredits
            ApplyViewImpl av (&*env.current(), tapNONE);
            PaymentSandbox pv (&av);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                pv, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            issueIOU (pv, alice, toCredit, iss, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount);
        }

        {
            // accountSend, w/ deferredCredits and stacked views
            ApplyViewImpl av (&*env.current(), tapNONE);
            PaymentSandbox pv (&av);

            auto const iss = USD_gw1.issue ();
            auto const startingAmount = accountHolds (
                pv, alice, iss.currency, iss.account, fhIGNORE_FREEZE, j);

            accountSend (pv, gw1, alice, toCredit, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount);

            {
                PaymentSandbox pv2(&pv);
                BEAST_EXPECT(accountHolds (pv2, alice, iss.currency, iss.account,
                            fhIGNORE_FREEZE, j) ==
                        startingAmount);
                accountSend (pv2, gw1, alice, toCredit, j);
                BEAST_EXPECT(accountHolds (pv2, alice, iss.currency, iss.account,
                            fhIGNORE_FREEZE, j) ==
                        startingAmount);
            }

            accountSend (pv, alice, gw1, toDebit, j);
            BEAST_EXPECT(accountHolds (pv, alice, iss.currency, iss.account,
                        fhIGNORE_FREEZE, j) ==
                    startingAmount - toDebit);
        }
    }

    void testTinyBalance ()
    {
        testcase ("Tiny balance");

        // Add and subtract a huge credit from a tiny balance, expect the tiny
        // balance back. Numerical stability problems could cause the balance to
        // be zero.

        using namespace jtx;

        Env env (*this);

        Account const gw ("gw");
        Account const alice ("alice");
        auto const USD = gw["USD"];

        auto const issue = USD.issue ();
        STAmount tinyAmt (issue, STAmount::cMinValue, STAmount::cMinOffset + 1,
            false, false, STAmount::unchecked{});
        STAmount hugeAmt (issue, STAmount::cMaxValue, STAmount::cMaxOffset - 1,
            false, false, STAmount::unchecked{});

        for (auto timeDelta : {-env.closed ()->info ().closeTimeResolution,
                 env.closed ()->info ().closeTimeResolution})
        {
            auto const closeTime = amendmentRIPD1141SoTime () + timeDelta;
            env.close (closeTime);
            ApplyViewImpl av (&*env.current (), tapNONE);
            PaymentSandbox pv (&av);
            pv.creditHook (gw, alice, hugeAmt, -tinyAmt);
            if (closeTime > amendmentRIPD1141SoTime ())
                BEAST_EXPECT(pv.balanceHook (alice, gw, hugeAmt) == tinyAmt);
            else
                BEAST_EXPECT(pv.balanceHook (alice, gw, hugeAmt) != tinyAmt);
        }
    }
    void testReserve()
    {
        testcase ("Reserve");
        using namespace jtx;

        beast::Journal dj;

        auto accountFundsM = [&dj](
            ReadView const& view, AccountID const& id) -> MAmount
        {
            return toAmount<MAmount> (accountHolds (
                view, id, mCurrency (), mAccount (), fhZERO_IF_FROZEN, dj));
        };

        auto reserve = [](jtx::Env& env, std::uint32_t count) -> MAmount
        {
            return env.current ()->fees ().accountReserve (count);
        };

        Env env (*this);

        Account const alice ("alice");
        env.fund (reserve(env, 1), alice);

        auto const closeTime = amendmentRIPD1141SoTime () +
                100 * env.closed ()->info ().closeTimeResolution;
        env.close (closeTime);
        ApplyViewImpl av (&*env.current (), tapNONE);
        PaymentSandbox sb (&av);
        {
            // Send alice an amount and spend it. The deferredCredits will cause her balance
            // to drop below the reserve. Make sure her funds are zero (there was a bug that
            // caused her funds to become negative).

            accountSend (sb, mAccount (), alice, M(100), dj);
            accountSend (sb, alice, mAccount (), M(100), dj);
            BEAST_EXPECT(accountFundsM (sb, alice) == beast::zero);
        }

    }

public:
    void run ()
    {
        testSelfFunding ();
        testSubtractCredits ();
        testTinyBalance ();
        testReserve();
    }
};

BEAST_DEFINE_TESTSUITE (PaymentSandbox, ledger, mtchain);

}  // test
}  //
