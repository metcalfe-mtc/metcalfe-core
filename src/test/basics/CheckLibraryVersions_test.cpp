//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/CheckLibraryVersions.h>
#include <mtchain/basics/impl/CheckLibraryVersionsImpl.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {
namespace version {

struct CheckLibraryVersions_test : beast::unit_test::suite
{
    void testBadOpenSSL()
    {
        testcase ("Out-of-Date OpenSSL");
        except ([&]{ checkOpenSSL("0.9.8-o"); });
        except ([&]{ checkOpenSSL("1.0.1-d"); });
        except ([&]{ checkOpenSSL("1.0.2-c"); });
    }

    void testBadBoost()
    {
        testcase ("Out-of-Date Boost");
        except ([&]{ checkBoost ("1.54.0"); });
    }

    void run()
    {
        unexcept ([&]{ checkLibraryVersions(); });

        testBadOpenSSL();
        testBadBoost();
    }
};

BEAST_DEFINE_TESTSUITE(CheckLibraryVersions, mtchain_basics, mtchain);

}  // namespace version
}  // namespace mtchain
