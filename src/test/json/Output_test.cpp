//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/json/json_reader.h>
#include <mtchain/json/json_writer.h>
#include <test/json/TestOutputSuite.h>

namespace Json {

struct Output_test : mtchain::test::TestOutputSuite
{
    void runTest (std::string const& name, std::string const& valueDesc)
    {
        setup (name);
        Json::Value value;
        BEAST_EXPECT(Json::Reader().parse (valueDesc, value));
        auto out = stringOutput (output_);
        outputJson (value, out);

        // Compare with the original version.
        auto expected = Json::FastWriter().write (value);
        expectResult (expected);
        expectResult (valueDesc);
        expectResult (jsonAsString (value));
    }

    void runTest (std::string const& name)
    {
        runTest (name, name);
    }

    void run () override
    {
        runTest ("empty dict", "{}");
        runTest ("empty array", "[]");
        runTest ("array", "[23,4.25,true,null,\"string\"]");
        runTest ("dict", "{\"hello\":\"world\"}");
        runTest ("array dict", "[{}]");
        runTest ("array array", "[[]]");
        runTest ("more complex",
                 "{\"array\":[{\"12\":23},{},null,false,0.5]}");
    }
};

BEAST_DEFINE_TESTSUITE(Output, mtchain_basics, mtchain);

} // Json
