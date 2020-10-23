//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/protocol/digest.h>
#include <mtchain/beast/utility/rngfill.h>
#include <mtchain/beast/xor_shift_engine.h>
#include <mtchain/beast/unit_test.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>
#include <vector>

namespace mtchain {

class digest_test : public beast::unit_test::suite
{
    std::vector<uint256> dataset1;

    template <class Hasher>
    void test (char const* name)
    {
        using namespace std::chrono;

        // Prime the cache
        for (int i = 0; i != 4; i++)
        {
            for (auto const& x : dataset1)
            {
                Hasher h;
                h (x.data (), x.size ());
                (void) static_cast<typename Hasher::result_type>(h);
            }
        }

        std::array<nanoseconds, 128> results;

        for (auto& result : results)
        {
            auto const start = high_resolution_clock::now ();

            for (auto const& x : dataset1)
            {
                Hasher h;
                h (x.data (), x.size ());
                (void) static_cast<typename Hasher::result_type>(h);
            }

            auto const d = high_resolution_clock::now () - start;

            result = d;
        }

        log << "    " << name << ":" << '\n';

        auto const sum = std::accumulate(
            results.begin(), results.end(),
            nanoseconds{0});
        {
            auto s = duration_cast<seconds>(sum);
            auto ms = duration_cast<milliseconds>(sum) - s;
            log <<
                "       Total Time = " << s.count() <<
                "." << ms.count() << " seconds" << std::endl;
        }

        auto const mean = sum / results.size();
        {
            auto s = duration_cast<seconds>(mean);
            auto ms = duration_cast<milliseconds>(mean) - s;
            log <<
                "        Mean Time = " << s.count() <<
                "." << ms.count() << " seconds" << std::endl;
        }

        std::vector<nanoseconds::rep> diff(results.size());
        std::transform(
            results.begin(), results.end(), diff.begin(),
            [&mean](nanoseconds trial)
            {
                return (trial - mean).count();
            });
        auto const sq_sum = std::inner_product(
            diff.begin(), diff.end(), diff.begin(), 0.0);
        {
            nanoseconds const stddev {
                static_cast<nanoseconds::rep>(
                    std::sqrt(sq_sum / results.size()))
            };
            auto s = duration_cast<seconds>(stddev);
            auto ms = duration_cast<milliseconds>(stddev) - s;
            log <<
                "          Std Dev = " << s.count() <<
                "." << ms.count() << " seconds" << std::endl;
        }
    }

public:
    digest_test ()
    {
        beast::xor_shift_engine g(19207813);
        std::uint8_t buf[32];

        for (int i = 0; i < 1000000; i++)
        {
            beast::rngfill (buf, sizeof(buf), g);
            dataset1.push_back (uint256::fromVoid (buf));
        }
    }

    void testSHA512 ()
    {
        testcase ("SHA512");
        test<openssl_ripemd160_hasher> ("OpenSSL");
        test<beast::ripemd160_hasher> ("Beast");
        pass ();
    }

    void testSHA256 ()
    {
        testcase ("SHA256");
        test<openssl_sha256_hasher> ("OpenSSL");
        test<beast::sha256_hasher> ("Beast");
        pass ();
    }

    void testRIPEMD160 ()
    {
        testcase ("RIPEMD160");
        test<openssl_ripemd160_hasher> ("OpenSSL");
        test<beast::ripemd160_hasher> ("Beast");
        pass ();
    }

    void run ()
    {
        testSHA512 ();
        testSHA256 ();
        testRIPEMD160 ();
    }
};

BEAST_DEFINE_TESTSUITE_MANUAL(digest,mtchain_data,mtchain);

} //
