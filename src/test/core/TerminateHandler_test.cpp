//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/core/TerminateHandler.h>
#include <mtchain/beast/core/CurrentThreadName.h>
#include <mtchain/beast/unit_test.h>

#include <boost/coroutine/exceptions.hpp>
#include <exception>
#include <sstream>
#include <streambuf>

namespace mtchain {
namespace test {

class TerminateHandler_test : public beast::unit_test::suite
{
private:
    // Allow cerr to be redirected.  Destructor restores old cerr streambuf.
    class CerrRedirect
    {
    public:
        CerrRedirect (std::stringstream& sStream)
        : old_ (std::cerr.rdbuf (sStream.rdbuf()))
        { }

        ~CerrRedirect()
        {
            std::cerr.rdbuf (old_);
        }

    private:
        std::streambuf* const old_;
    };

    // Set a new current thread name.  Destructor restores the old thread name.
    class ThreadNameGuard
    {
    public:
        ThreadNameGuard (std::string const& newName)
        : old_ (beast::getCurrentThreadName ())
        {
            beast::setCurrentThreadName (newName);
        }

        ~ThreadNameGuard()
        {
            std::string oldName;
            if (old_)
                oldName = std::move (*old_);

            beast::setCurrentThreadName (oldName);
        }

    private:
        boost::optional<std::string> old_;
    };

public:
    void
    run() override
    {
        // Set the current thread name, but restore the old name on exit.
        std::string const threadName {"terminateHandler_test"};
        ThreadNameGuard nameGuard {threadName};
        {
            // Test terminateHandler() with a std::exception.

            // The terminateHandler() output goes to std::cerr.  Capture that.
            std::stringstream cerrCapture;
            CerrRedirect cerrRedirect {cerrCapture};

            try
            {
                throw std::range_error ("Out of range");
            }
            catch (...)
            {
                terminateHandler();
            }
            {
                std::string result = cerrCapture.str();
                BEAST_EXPECT (result.find (threadName) != std::string::npos);
                BEAST_EXPECT (
                    result.find ("Out of range") != std::string::npos);
            }
        }

        {
            // Verify terminateHnadler() handles forced_unwind correctly.
            std::stringstream cerrCapture;
            CerrRedirect cerrRedirect {cerrCapture};

            try
            {
                throw boost::coroutines::detail::forced_unwind();
            }
            catch (...)
            {
                terminateHandler();
            }
            {
                std::string result = cerrCapture.str();
                BEAST_EXPECT (result.find (threadName) != std::string::npos);
                BEAST_EXPECT (
                    result.find ("forced_unwind") != std::string::npos);
            }
        }

        {
            // Verify terminatHandler()'s handling of non-standard exceptions.
            std::stringstream cerrCapture;
            CerrRedirect cerrRedirect {cerrCapture};

            try
            {
                throw 7;
            }
            catch (...)
            {
                terminateHandler();
            }
            {
                std::string result = cerrCapture.str();
                BEAST_EXPECT (result.find (threadName) != std::string::npos);
                BEAST_EXPECT (
                    result.find ("unknown exception") != std::string::npos);
            }
        }
    }
};

BEAST_DEFINE_TESTSUITE(TerminateHandler,core,mtchain);

} // test
} //
