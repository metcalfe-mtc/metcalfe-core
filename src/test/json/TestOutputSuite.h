//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_RPC_TESTOUTPUTSUITE_H_INCLUDED
#define MTCHAIN_RPC_TESTOUTPUTSUITE_H_INCLUDED

#include <mtchain/json/Output.h>
#include <mtchain/json/Writer.h>
#include <test/jtx/TestSuite.h>

namespace mtchain {
namespace test {

class TestOutputSuite : public TestSuite
{
protected:
    std::string output_;
    std::unique_ptr <Json::Writer> writer_;

    void setup (std::string const& testName)
    {
        testcase (testName);
        output_.clear ();
        writer_ = std::make_unique <Json::Writer> (
            Json::stringOutput (output_));
    }

    // Test the result and report values.
    void expectResult (std::string const& expected,
                       std::string const& message = "")
    {
        writer_.reset ();

        expectEquals (output_, expected, message);
    }
};

} // test
} //

#endif
