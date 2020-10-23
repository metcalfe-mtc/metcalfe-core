//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.
    Permission to use, copy, modify, and/or distribute this software for any
*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx.h>
#include <mtchain/ledger/Directory.h>

namespace mtchain {
namespace test {

struct Directory_test : public beast::unit_test::suite
{
    void testDirectory()
    {
        using namespace jtx;
        Env env(*this);
        auto gw = Account("gw");
        auto USD = gw["USD"];

        {
            auto dir = Dir(*env.current(),
                keylet::ownerDir(Account("alice")));
            BEAST_EXPECT(std::begin(dir) == std::end(dir));
            BEAST_EXPECT(std::end(dir) == dir.find(uint256(), uint256()));
        }

        env.fund(M(10000), "alice", "bob", gw);

        auto i = 10;
        for (; i <= 400; i += 10)
            env(offer("alice", USD(i), M(10)));
        env(offer("bob", USD(500), M(10)));

        {
            auto dir = Dir(*env.current(),
                keylet::ownerDir(Account("bob")));
            BEAST_EXPECT(std::begin(dir)->get()->
                getFieldAmount(sfTakerPays) == USD(500));
        }

        auto dir = Dir(*env.current(),
            keylet::ownerDir(Account("alice")));
        i = 0;
        for (auto const& e : dir)
            BEAST_EXPECT(e->getFieldAmount(sfTakerPays) == USD(i += 10));

        BEAST_EXPECT(std::begin(dir) != std::end(dir));
        BEAST_EXPECT(std::end(dir) ==
            dir.find(std::begin(dir).page().key,
                uint256()));
        BEAST_EXPECT(std::begin(dir) ==
            dir.find(std::begin(dir).page().key,
                std::begin(dir).index()));
        auto entry = std::next(std::begin(dir), 32);
        auto it = dir.find(entry.page().key, entry.index());
        BEAST_EXPECT(it != std::end(dir));
        BEAST_EXPECT((*it)->getFieldAmount(sfTakerPays) == USD(330));
    }

    void run() override
    {
        testDirectory();
    }
};

BEAST_DEFINE_TESTSUITE(Directory,ledger,mtchain);

} // test
} //
