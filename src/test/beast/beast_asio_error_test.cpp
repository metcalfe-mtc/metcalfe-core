//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <mtchain/beast/asio/ssl_error.h>
#include <mtchain/beast/unit_test.h>
#include <string>

namespace beast {

class error_test : public unit_test::suite
{
public:
    void run()
    {
        {
            boost::system::error_code ec =
                boost::system::error_code (335544539,
                    boost::asio::error::get_ssl_category ());
            std::string const s = beast::error_message_with_ssl(ec);
            BEAST_EXPECT(s == " (20,0,219) error:140000DB:SSL routines:SSL routines:short read");
        }
    }
};

BEAST_DEFINE_TESTSUITE(error,asio,beast);

} // beast
