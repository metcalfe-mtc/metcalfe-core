//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/protocol/MAmount.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

class MAmount_test : public beast::unit_test::suite
{
public:
    void testSigNum ()
    {
        testcase ("signum");

        for (auto i : { -1, 0, 1})
        {
            MAmount const x(i);

            if (i < 0)
                BEAST_EXPECT(x.signum () < 0);
            else if (i > 0)
                BEAST_EXPECT(x.signum () > 0);
            else
                BEAST_EXPECT(x.signum () == 0);
        }
    }

    void testBeastZero ()
    {
        testcase ("beast::Zero Comparisons");

        for (auto i : { -1, 0, 1})
        {
            MAmount const x (i);

            BEAST_EXPECT((i == 0) == (x == zero));
            BEAST_EXPECT((i != 0) == (x != zero));
            BEAST_EXPECT((i < 0) == (x < zero));
            BEAST_EXPECT((i > 0) == (x > zero));
            BEAST_EXPECT((i <= 0) == (x <= zero));
            BEAST_EXPECT((i >= 0) == (x >= zero));

            BEAST_EXPECT((0 == i) == (zero == x));
            BEAST_EXPECT((0 != i) == (zero != x));
            BEAST_EXPECT((0 < i) == (zero < x));
            BEAST_EXPECT((0 > i) == (zero > x));
            BEAST_EXPECT((0 <= i) == (zero <= x));
            BEAST_EXPECT((0 >= i) == (zero >= x));
        }
    }

    void testComparisons ()
    {
        testcase ("M Comparisons");

        for (auto i : { -1, 0, 1})
        {
            MAmount const x (i);

            for (auto j : { -1, 0, 1})
            {
                MAmount const y (j);

                BEAST_EXPECT((i == j) == (x == y));
                BEAST_EXPECT((i != j) == (x != y));
                BEAST_EXPECT((i < j) == (x < y));
                BEAST_EXPECT((i > j) == (x > y));
                BEAST_EXPECT((i <= j) == (x <= y));
                BEAST_EXPECT((i >= j) == (x >= y));
            }
        }
    }

    void testAddSub ()
    {
        testcase ("Addition & Subtraction");

        for (auto i : { -1, 0, 1})
        {
            MAmount const x (i);

            for (auto j : { -1, 0, 1})
            {
                MAmount const y (j);

                BEAST_EXPECT(MAmount(i + j) == (x + y));
                BEAST_EXPECT(MAmount(i - j) == (x - y));

                BEAST_EXPECT((x + y) == (y + x));   // addition is commutative
            }
        }
    }

    void testMulRatio()
    {
        testcase ("mulRatio");

        constexpr auto maxUInt32 = std::numeric_limits<std::uint32_t>::max ();
        constexpr auto maxUInt64 = std::numeric_limits<std::uint64_t>::max ();

        {
            // multiply by a number that would overflow then divide by the same
            // number, and check we didn't lose any value
            MAmount big (maxUInt64);
            BEAST_EXPECT(big == mulRatio (big, maxUInt32, maxUInt32, true));
            // rounding mode shouldn't matter as the result is exact
            BEAST_EXPECT(big == mulRatio (big, maxUInt32, maxUInt32, false));
        }

        {
            // Similar test as above, but for neative values
            MAmount big (maxUInt64);
            BEAST_EXPECT(big == mulRatio (big, maxUInt32, maxUInt32, true));
            // rounding mode shouldn't matter as the result is exact
            BEAST_EXPECT(big == mulRatio (big, maxUInt32, maxUInt32, false));
        }

        {
            // small amounts
            MAmount tiny (1);
            // Round up should give the smallest allowable number
            BEAST_EXPECT(tiny == mulRatio (tiny, 1, maxUInt32, true));
            // rounding down should be zero
            BEAST_EXPECT(beast::zero == mulRatio (tiny, 1, maxUInt32, false));
            BEAST_EXPECT(beast::zero ==
                mulRatio (tiny, maxUInt32 - 1, maxUInt32, false));

            // tiny negative numbers
            MAmount tinyNeg (-1);
            // Round up should give zero
            BEAST_EXPECT(zero == mulRatio (tinyNeg, 1, maxUInt32, true));
            BEAST_EXPECT(zero == mulRatio (tinyNeg, maxUInt32 - 1, maxUInt32, true));
            // rounding down should be tiny
            BEAST_EXPECT(tinyNeg == mulRatio (tinyNeg, maxUInt32 - 1, maxUInt32, false));
        }

        {
            // rounding
            {
                MAmount one (1);
                auto const rup = mulRatio (one, maxUInt32 - 1, maxUInt32, true);
                auto const rdown = mulRatio (one, maxUInt32 - 1, maxUInt32, false);
                BEAST_EXPECT(rup.drops () - rdown.drops () == 1);
            }

            {
                MAmount big (maxUInt64);
                auto const rup = mulRatio (big, maxUInt32 - 1, maxUInt32, true);
                auto const rdown = mulRatio (big, maxUInt32 - 1, maxUInt32, false);
                BEAST_EXPECT(rup.drops () - rdown.drops () == 1);
            }

            {
                MAmount negOne (-1);
                auto const rup = mulRatio (negOne, maxUInt32 - 1, maxUInt32, true);
                auto const rdown = mulRatio (negOne, maxUInt32 - 1, maxUInt32, false);
                BEAST_EXPECT(rup.drops () - rdown.drops () == 1);
            }
        }

        {
            // division by zero
            MAmount one (1);
            except ([&] {mulRatio (one, 1, 0, true);});
        }

        {
            // overflow
            MAmount big (maxUInt64);
            except ([&] {mulRatio (big, 2, 0, true);});
        }
    }

    //--------------------------------------------------------------------------

    void run ()
    {
        testSigNum ();
        testBeastZero ();
        testComparisons ();
        testAddSub ();
        testMulRatio ();
    }
};

BEAST_DEFINE_TESTSUITE(MAmount,protocol,mtchain);

} //
