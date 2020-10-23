//------------------------------------------------------------------------------
/*
This file is part of mtchaind: https://github.com/MTChain/MTChain-core
Copyright (c) 2017, 2018 MTChain Alliance.

Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <mtchain/core/impl/Workers.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {

//------------------------------------------------------------------------------

class Workers_test : public beast::unit_test::suite
{
public:
    struct TestCallback : Workers::Callback
    {
        TestCallback()
            : finished(false, false)
            , count(0)
        {
        }

        void processTask()
        {
            if (--count == 0)
                finished.signal();
        }

        beast::WaitableEvent finished;
        std::atomic <int> count;
    };

    void testThreads(int const tc1, int const tc2, int const tc3)
    {
        testcase("threadCounts: " + std::to_string(tc1) +
            " -> " + std::to_string(tc2) + " -> " + std::to_string(tc3));

        TestCallback cb;

        Workers w(cb, "Test", tc1);
        BEAST_EXPECT(w.getNumberOfThreads() == tc1);

        auto testForThreadCount = [this, &cb, &w] (int const threadCount)
        {
            // Prepare the callback.
            cb.count = threadCount;
            if (threadCount == 0)
                cb.finished.signal();
            else
                cb.finished.reset();

            // Execute the test.
            w.setNumberOfThreads(threadCount);
            BEAST_EXPECT(w.getNumberOfThreads() == threadCount);

            for (int i = 0; i < threadCount; ++i)
                w.addTask();

            // 10 seconds should be enough to finish on any system
            //
            bool const signaled = cb.finished.wait(10 * 1000);
            BEAST_EXPECT(signaled);
            BEAST_EXPECT(cb.count.load() == 0);
        };
        testForThreadCount (tc1);
        testForThreadCount (tc2);
        testForThreadCount (tc3);
        w.pauseAllThreadsAndWait();

        // We had better finished all our work!
        BEAST_EXPECT(cb.count.load() == 0);
    }

    void run()
    {
        testThreads( 0, 0,  0);
        testThreads( 1, 0,  1);
        testThreads( 2, 1,  2);
        testThreads( 4, 3,  5);
        testThreads(16, 4, 15);
        testThreads(64, 3, 65);
    }
};

BEAST_DEFINE_TESTSUITE(Workers, core, mtchain);

}
