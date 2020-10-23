//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2012-2016 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/protocol/JsonFields.h>
#include <test/jtx.h>
#include <mtchain/beast/unit_test.h>

#include <boost/format.hpp>

namespace mtchain {

namespace test {

namespace validator {
static auto const public_key =
    "nHBt9fsb4849WmZiCds4r5TXyBeQjqnH5kzPtqgMAQMgi39YZRPa";

static auto const token =
    "eyJ2YWxpZGF0aW9uX3NlY3JldF9rZXkiOiI5ZWQ0NWY4NjYyNDFjYzE4YTI3NDdiNT\n"
    "QzODdjMDYyNTkwNzk3MmY0ZTcxOTAyMzFmYWE5Mzc0NTdmYTlkYWY2IiwibWFuaWZl\n"
    "c3QiOiJKQUFBQUFGeEllMUZ0d21pbXZHdEgyaUNjTUpxQzlnVkZLaWxHZncxL3ZDeE\n"
    "hYWExwbGMyR25NaEFrRTFhZ3FYeEJ3RHdEYklENk9NU1l1TTBGREFscEFnTms4U0tG\n"
    "bjdNTzJmZGtjd1JRSWhBT25ndTlzQUtxWFlvdUorbDJWMFcrc0FPa1ZCK1pSUzZQU2\n"
    "hsSkFmVXNYZkFpQnNWSkdlc2FhZE9KYy9hQVpva1MxdnltR21WcmxIUEtXWDNZeXd1\n"
    "NmluOEhBU1FLUHVnQkQ2N2tNYVJGR3ZtcEFUSGxHS0pkdkRGbFdQWXk1QXFEZWRGdj\n"
    "VUSmEydzBpMjFlcTNNWXl3TFZKWm5GT3I3QzBrdzJBaVR6U0NqSXpkaXRROD0ifQ==\n";
}

class ServerInfo_test : public beast::unit_test::suite
{
public:
    static
    std::unique_ptr<Config>
    makeValidatorConfig()
    {
        auto p = std::make_unique<Config>();
        boost::format toLoad(R"mtchainConfig(
[validator_token]
%1%

[validators]
%2%
)mtchainConfig");

        p->loadFromString (boost::str (
            toLoad % validator::token % validator::public_key));

        setupConfigForUnitTests(*p);

        return p;
    }

    void testServerInfo()
    {
        using namespace test::jtx;

        {
            Env env(*this);
            auto const result = env.rpc("server_info", "1");
            BEAST_EXPECT(!result[jss::result].isMember (jss::error));
            BEAST_EXPECT(result[jss::status] == "success");
            BEAST_EXPECT(result[jss::result].isMember(jss::info));
        }
        {
            Env env(*this, makeValidatorConfig());
            auto const result = env.rpc("server_info", "1");
            BEAST_EXPECT(!result[jss::result].isMember (jss::error));
            BEAST_EXPECT(result[jss::status] == "success");
            BEAST_EXPECT(result[jss::result].isMember(jss::info));
            BEAST_EXPECT(result[jss::result][jss::info]
                [jss::pubkey_validator] == validator::public_key);
        }
    }

    void run ()
    {
        testServerInfo ();
    }
};

BEAST_DEFINE_TESTSUITE(ServerInfo,app,mtchain);

} // test
} //

