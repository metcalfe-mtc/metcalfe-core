//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.
    Permission to use, copy, modify, and/or distribute this software for any
*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx.h>
#include <mtchain/ledger/BookDirs.h>

namespace mtchain {
namespace test {

struct BookDirs_test : public beast::unit_test::suite
{
    void test_bookdir()
    {
        using namespace jtx;
        Env env(*this);
        auto gw = Account("gw");
        auto USD = gw["USD"];
        env.fund(M(1000000), "alice", "bob", "gw");

        {
            Book book(mIssue(), USD.issue());
            {
                auto d = BookDirs(*env.current(), book);
                BEAST_EXPECT(std::begin(d) == std::end(d));
                BEAST_EXPECT(std::distance(d.begin(), d.end()) == 0);
            }
            {
                auto d = BookDirs(*env.current(), reversed(book));
                BEAST_EXPECT(std::distance(d.begin(), d.end()) == 0);
            }
        }

        {
            env(offer("alice", Account("alice")["USD"](50), M(10)));
            auto d = BookDirs(*env.current(),
                Book(Account("alice")["USD"].issue(), mIssue()));
            BEAST_EXPECT(std::distance(d.begin(), d.end()) == 1);
        }

        {
            env(offer("alice", gw["CNY"](50), M(10)));
            auto d = BookDirs(*env.current(),
                Book(gw["CNY"].issue(), mIssue()));
            BEAST_EXPECT(std::distance(d.begin(), d.end()) == 1);
        }

        {
            env.trust(Account("bob")["CNY"](10), "alice");
            env(pay("bob", "alice", Account("bob")["CNY"](10)));
            env(offer("alice", USD(50), Account("bob")["CNY"](10)));
            auto d = BookDirs(*env.current(),
                Book(USD.issue(), Account("bob")["CNY"].issue()));
            BEAST_EXPECT(std::distance(d.begin(), d.end()) == 1);
        }

        {
            auto AUD = gw["AUD"];
            for (auto i = 1, j = 3; i <= 3; ++i, --j)
                for (auto k = 0; k < 80; ++k)
                    env(offer("alice", AUD(i), M(j)));

            auto d = BookDirs(*env.current(),
                Book(AUD.issue(), mIssue()));
            BEAST_EXPECT(std::distance(d.begin(), d.end()) == 240);
            auto i = 1, j = 3, k = 0;
            for (auto const& e : d)
            {
                BEAST_EXPECT(e->getFieldAmount(sfTakerPays) == AUD(i));
                BEAST_EXPECT(e->getFieldAmount(sfTakerGets) == M(j));
                if (++k % 80 == 0)
                {
                    ++i;
                    --j;
                }
            }
        }
    }

    void run() override
    {
        test_bookdir();
    }
};

BEAST_DEFINE_TESTSUITE(BookDirs,ledger,mtchain);

} // test
} //
