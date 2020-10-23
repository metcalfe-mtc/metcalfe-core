//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright 2014 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/overlay/impl/TMHello.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

class TMHello_test : public beast::unit_test::suite
{
private:
    template <class FwdIt>
    static
    std::string
    join (FwdIt first, FwdIt last, char c = ',')
    {
        std::string result;
        if (first == last)
            return result;
        result = to_string(*first++);
        while(first != last)
            result += "," + to_string(*first++);
        return result;
    }

    void
    check(std::string const& s, std::string const& answer)
    {
        auto const result = parse_ProtocolVersions(s);
        BEAST_EXPECT(join(result.begin(), result.end()) == answer);
    }

public:
    void
    test_protocolVersions()
    {
        check("", "");
        check("RTXP/1.0", "1.0");
        check("RTXP/1.0, Websocket/1.0", "1.0");
        check("RTXP/1.0, RTXP/1.0", "1.0");
        check("RTXP/1.0, RTXP/1.1", "1.0,1.1");
        check("RTXP/1.1, RTXP/1.0", "1.0,1.1");
    }

    void
    run()
    {
        test_protocolVersions();
    }
};

BEAST_DEFINE_TESTSUITE(TMHello,overlay,mtchain);

}
