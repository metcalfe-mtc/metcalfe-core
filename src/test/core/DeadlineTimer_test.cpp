//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <mtchain/core/DeadlineTimer.h>
#include <mtchain/beast/unit_test.h>
#include <atomic>
#include <chrono>
#include <thread>

namespace mtchain {

//------------------------------------------------------------------------------

class DeadlineTimer_test : public beast::unit_test::suite
{
public:
    struct TestCallback : DeadlineTimer::Listener
    {
        TestCallback() = default;

        void onDeadlineTimer (DeadlineTimer&) override
        {
            ++count;
        }

        std::atomic<int> count {0};
    };

    void testExpiration()
    {
        using clock = DeadlineTimer::clock;

        using namespace std::chrono_literals;
        using namespace std::this_thread;

        TestCallback cb;
        DeadlineTimer dt {&cb};

        // There are parts of this test that are somewhat race conditional.
        // The test is designed to avoid spurious failures, rather than
        // fail occasionally but randomly, where ever possible.  So there may
        // be occasional gratuitous passes.  Unfortunately, since it is a
        // time-based test, there may also be occasional spurious failures
        // on low-powered continuous integration platforms.
        {
            testcase("Expiration");

            // Set a deadline timer that should only fire once in 5ms.
            cb.count = 0;
            auto const startTime = clock::now();
            dt.setExpiration (5ms);

             // Make sure the timer didn't fire immediately.
            int const count = cb.count.load();
            if (clock::now() < startTime + 4ms)
            {
                BEAST_EXPECT (count == 0);
            }

            // Wait until the timer should have fired and check that it did.
            // In fact, we wait long enough that if it were to fire multiple
            // times we'd see that.
            sleep_until (startTime + 50ms);
            BEAST_EXPECT (cb.count.load() == 1);
        }
        {
            testcase("RecurringExpiration");

            // Set a deadline timer that should fire once every 5ms.
            cb.count = 0;
            auto const startTime = clock::now();
            dt.setRecurringExpiration (5ms);

             // Make sure the timer didn't fire immediately.
            {
                int const count = cb.count.load();
                if (clock::now() < startTime + 4ms)
                {
                    BEAST_EXPECT (count == 0);
                }
            }

            // Wait until the timer should have fired several times and
            // check that it did.
            sleep_until (startTime + 100ms);
            {
                auto const count = cb.count.load();
                BEAST_EXPECT ((count > 1) && (count < 21));
            }

            // Cancel the recurring timer and it should not fire any more.
            dt.cancel();
            auto const count = cb.count.load();
            sleep_for (50ms);
            BEAST_EXPECT (count == cb.count.load());
        }
    }

    void run()
    {
        testExpiration();
    }
};

BEAST_DEFINE_TESTSUITE(DeadlineTimer, core, mtchain);

}