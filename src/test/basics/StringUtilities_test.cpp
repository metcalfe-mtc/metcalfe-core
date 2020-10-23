//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/Slice.h>
#include <mtchain/basics/StringUtilities.h>
#include <mtchain/basics/ToString.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

class StringUtilities_test : public beast::unit_test::suite
{
public:
    void testUnHexSuccess (std::string const& strIn, std::string const& strExpected)
    {
        auto rv = strUnHex (strIn);
        BEAST_EXPECT(rv.second);
        BEAST_EXPECT(makeSlice(rv.first) == makeSlice(strExpected));
    }

    void testUnHexFailure (std::string const& strIn)
    {
        auto rv = strUnHex (strIn);
        BEAST_EXPECT(! rv.second);
        BEAST_EXPECT(rv.first.empty());
    }

    void testUnHex ()
    {
        testcase ("strUnHex");

        testUnHexSuccess ("526970706c6544", "MTChainD");
        testUnHexSuccess ("A", "\n");
        testUnHexSuccess ("0A", "\n");
        testUnHexSuccess ("D0A", "\r\n");
        testUnHexSuccess ("0D0A", "\r\n");
        testUnHexSuccess ("200D0A", " \r\n");
        testUnHexSuccess ("282A2B2C2D2E2F29", "(*+,-./)");

        // Check for things which contain some or only invalid characters
        testUnHexFailure ("123X");
        testUnHexFailure ("V");
        testUnHexFailure ("M");
    }

    void testParseUrl ()
    {
        testcase ("parseUrl");

        parsedURL pUrl;

        BEAST_EXPECT(parseUrl (pUrl, "lower://domain"));
        BEAST_EXPECT(pUrl.scheme == "lower");
        BEAST_EXPECT(pUrl.domain == "domain");
        BEAST_EXPECT(! pUrl.port);
        BEAST_EXPECT(pUrl.path == "");
        BEAST_EXPECT(parseUrl (pUrl, "UPPER://domain:234/"));
        BEAST_EXPECT(pUrl.scheme == "upper");
        BEAST_EXPECT(*pUrl.port == 234);
        BEAST_EXPECT(pUrl.path == "/");
        BEAST_EXPECT(parseUrl (pUrl, "Mixed://domain/path"));
        BEAST_EXPECT(pUrl.scheme == "mixed");
        BEAST_EXPECT(pUrl.path == "/path");
    }

    void testToString ()
    {
        testcase ("toString");
        auto result = to_string("hello");
        BEAST_EXPECT(result == "hello");
    }

    void run ()
    {
        testParseUrl ();
        testUnHex ();
        testToString ();
    }
};

BEAST_DEFINE_TESTSUITE(StringUtilities, mtchain_basics, mtchain);

} //
