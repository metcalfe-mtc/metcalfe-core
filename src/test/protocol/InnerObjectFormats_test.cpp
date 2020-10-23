//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/contract.h>
#include <mtchain/protocol/InnerObjectFormats.h>
#include <mtchain/protocol/ErrorCodes.h>          // RPC::containsError
#include <mtchain/json/json_reader.h>             // Json::Reader
#include <mtchain/protocol/STParsedJSON.h>        // STParsedJSONObject
#include <mtchain/beast/unit_test.h>
#include <test/jtx.h>

namespace mtchain {

namespace InnerObjectFormatsUnitTestDetail
{

struct TestJSONTxt
{
    std::string const txt;
    bool const expectFail;
};

static TestJSONTxt const testArray[] =
{

// Valid SignerEntry
{R"({
    "Account" : "rDg53Haik2475DJx8bjMDSDPj4VX7htaMd",
    "SignerEntries" :
    [
        {
            "SignerEntry" :
            {
                "Account" : "rnUy2SHTrB9DubsPmkJZUXTf5FcNDGrYEA",
                "SignerWeight" : 4
            }
        },
        {
            "SignerEntry" :
            {
                "Account" : "rPcNzota6B8YBokhYtcTNqQVCngtbnWfux",
                "SignerWeight" : 3
            }
        }
    ],
    "SignerQuorum" : 7,
    "TransactionType" : "SignerListSet"
})", false
},

// SignerEntry missing Account
{R"({
    "Account" : "rDg53Haik2475DJx8bjMDSDPj4VX7htaMd",
    "SignerEntries" :
    [
        {
            "SignerEntry" :
            {
                "Account" : "rnUy2SHTrB9DubsPmkJZUXTf5FcNDGrYEA",
                "SignerWeight" : 4
            }
        },
        {
            "SignerEntry" :
            {
                "SignerWeight" : 3
            }
        }
    ],
    "SignerQuorum" : 7,
    "TransactionType" : "SignerListSet"
})", true
},

// SignerEntry missing SignerWeight
{R"({
    "Account" : "rDg53Haik2475DJx8bjMDSDPj4VX7htaMd",
    "SignerEntries" :
    [
        {
            "SignerEntry" :
            {
                "Account" : "rnUy2SHTrB9DubsPmkJZUXTf5FcNDGrYEA",
                "SignerWeight" : 4
            }
        },
        {
            "SignerEntry" :
            {
                "Account" : "rPcNzota6B8YBokhYtcTNqQVCngtbnWfux",
            }
        }
    ],
    "SignerQuorum" : 7,
    "TransactionType" : "SignerListSet"
})", true
},

// SignerEntry with unexpected Amount
{R"({
    "Account" : "rDg53Haik2475DJx8bjMDSDPj4VX7htaMd",
    "SignerEntries" :
    [
        {
            "SignerEntry" :
            {
                "Account" : "rnUy2SHTrB9DubsPmkJZUXTf5FcNDGrYEA",
                "SignerWeight" : 4
            }
        },
        {
            "SignerEntry" :
            {
                "Amount" : "1000000",
                "Account" : "rPcNzota6B8YBokhYtcTNqQVCngtbnWfux",
                "SignerWeight" : 3
            }
        }
    ],
    "SignerQuorum" : 7,
    "TransactionType" : "SignerListSet"
})", true
},

// SignerEntry with no Account and unexpected Amount
{R"({
    "Account" : "rDg53Haik2475DJx8bjMDSDPj4VX7htaMd",
    "SignerEntries" :
    [
        {
            "SignerEntry" :
            {
                "Account" : "rnUy2SHTrB9DubsPmkJZUXTf5FcNDGrYEA",
                "SignerWeight" : 4
            }
        },
        {
            "SignerEntry" :
            {
                "Amount" : "10000000",
                "SignerWeight" : 3
            }
        }
    ],
    "SignerQuorum" : 7,
    "TransactionType" : "SignerListSet"
})", true
},

};

} // namespace InnerObjectFormatsUnitTestDetail


class InnerObjectFormatsParsedJSON_test : public beast::unit_test::suite
{
public:
    void run()
    {
        using namespace InnerObjectFormatsUnitTestDetail;

        // Instantiate a jtx::Env so debugLog writes are exercised.
        test::jtx::Env env (*this);

        for (auto const& test : testArray)
        {
            Json::Value req;
            Json::Reader ().parse (test.txt, req);
            if (RPC::contains_error (req))
            {
                Throw<std::runtime_error> (
                    "Internal InnerObjectFormatsParsedJSON error.  Bad JSON.");
            }
            STParsedJSONObject parsed ("request", req);
            bool const noObj = parsed.object == boost::none;
            if ( noObj == test.expectFail )
            {
                pass ();
            }
            else
            {
                std::string errStr ("Unexpected STParsedJSON result on:\n");
                errStr += test.txt;
                fail (errStr);
            }
        }
    }
};

BEAST_DEFINE_TESTSUITE(InnerObjectFormatsParsedJSON,mtchain_app,mtchain);

} //
