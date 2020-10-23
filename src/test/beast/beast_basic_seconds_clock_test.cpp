//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <mtchain/beast/unit_test.h>

#include <mtchain/beast/clock/basic_seconds_clock.h>

namespace beast {

class basic_seconds_clock_test : public unit_test::suite
{
public:
    void
    run()
    {
        basic_seconds_clock <
            std::chrono::steady_clock>::now ();
        pass ();
    }
};

BEAST_DEFINE_TESTSUITE(basic_seconds_clock,chrono,beast);

}
