//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/basics/make_SSLContext.h>
#include <mtchain/server/Server.h>
#include <mtchain/server/Session.h>
#include <mtchain/beast/unit_test.h>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <chrono>
#include <stdexcept>
#include <thread>

namespace mtchain {
namespace test {

class Server_test : public beast::unit_test::suite
{
public:
    enum
    {
        testPort = 40000
    };

    class TestThread
    {
    private:
        boost::asio::io_service io_service_;
        boost::optional<boost::asio::io_service::work> work_;
        std::thread thread_;

    public:
        TestThread()
            : work_(boost::in_place(std::ref(io_service_)))
            , thread_([&]() { this->io_service_.run(); })
        {
        }

        ~TestThread()
        {
            work_ = boost::none;
            thread_.join();
        }

        boost::asio::io_service&
        get_io_service()
        {
            return io_service_;
        }
    };

    //--------------------------------------------------------------------------

    class TestSink : public beast::Journal::Sink
    {
        beast::unit_test::suite& suite_;

    public:
        TestSink (beast::unit_test::suite& suite)
            : Sink (beast::severities::kWarning, false)
            , suite_ (suite)
        {
        }

        void
        write (beast::severities::Severity level,
            std::string const& text) override
        {
            if (level < threshold())
                return;

            suite_.log << text;
        }
    };

    //--------------------------------------------------------------------------

    struct TestHandler
    {
        bool
        onAccept (Session& session,
            boost::asio::ip::tcp::endpoint endpoint)
        {
            return true;
        }

        Handoff
        onHandoff (Session& session,
            std::unique_ptr <beast::asio::ssl_bundle>&& bundle,
                http_request_type&& request,
                    boost::asio::ip::tcp::endpoint remote_address)
        {
            return Handoff{};
        }

        Handoff
        onHandoff (Session& session, boost::asio::ip::tcp::socket&& socket,
            http_request_type&& request,
                boost::asio::ip::tcp::endpoint remote_address)
        {
            return Handoff{};
        }

        void
        onRequest (Session& session)
        {
            session.write (std::string ("Hello, world!\n"));
            if (is_keep_alive(session.request()))
                session.complete();
            else
                session.close (true);
        }

        void
        onWSMessage(std::shared_ptr<WSSession> session,
            std::vector<boost::asio::const_buffer> const&)
        {
        }

        void
        onClose (Session& session,
            boost::system::error_code const&)
        {
        }

        void
        onStopped (Server& server)
        {
        }
    };

    //--------------------------------------------------------------------------

    // Connect to an address
    template <class Socket>
    bool
    connect (Socket& s, std::string const& addr, int port)
    {
        try
        {
            typename Socket::endpoint_type ep (
                boost::asio::ip::address::from_string (addr), port);
            s.connect (ep);
            pass();
            return true;
        }
        catch (std::exception const& e)
        {
            fail (e.what());
        }

        return false;
    }

    // Write a string to the stream
    template <class SyncWriteStream>
    bool
    write (SyncWriteStream& s, std::string const& text)
    {
        try
        {
            boost::asio::write (s, boost::asio::buffer (text));
            pass();
            return true;
        }
        catch (std::exception const& e)
        {
            fail (e.what());
        }
        return false;
    }

    // Expect that reading the stream produces a matching string
    template <class SyncReadStream>
    bool
    expect_read (SyncReadStream& s, std::string const& match)
    {
        boost::asio::streambuf b (1000); // limit on read
        try
        {
            auto const n = boost::asio::read_until (s, b, '\n');
            if (BEAST_EXPECT(n == match.size()))
            {
                std::string got;
                got.resize (n);
                boost::asio::buffer_copy (boost::asio::buffer (
                    &got[0], n), b.data());
                return BEAST_EXPECT(got == match);
            }
        }
        catch (std::length_error const& e)
        {
            fail(e.what());
        }
        catch (std::exception const& e)
        {
            fail(e.what());
        }
        return false;
    }

    void
    test_request()
    {
        boost::asio::io_service ios;
        using socket = boost::asio::ip::tcp::socket;
        socket s (ios);

        if (! connect (s, "127.0.0.1", testPort))
            return;

        if (! write (s,
            "GET / HTTP/1.1\r\n"
            "Connection: close\r\n"
            "\r\n"))
            return;

        if (! expect_read (s, "Hello, world!\n"))
            return ;

        boost::system::error_code ec;
        s.shutdown(socket::shutdown_both, ec);

        std::this_thread::sleep_for (std::chrono::seconds (1));
    }

    void
    test_keepalive()
    {
        boost::asio::io_service ios;
        using socket = boost::asio::ip::tcp::socket;
        socket s (ios);

        if (! connect (s, "127.0.0.1", testPort))
            return;

        if (! write (s,
            "GET / HTTP/1.1\r\n"
            "Connection: Keep-Alive\r\n"
            "\r\n"))
            return;

        if (! expect_read (s, "Hello, world!\n"))
            return ;

        if (! write (s,
            "GET / HTTP/1.1\r\n"
            "Connection: close\r\n"
            "\r\n"))
            return;

        if (! expect_read (s, "Hello, world!\n"))
            return ;

        boost::system::error_code ec;
        s.shutdown(socket::shutdown_both, ec);
    }

    void basicTests()
    {
        TestSink sink {*this};
        TestThread thread;
        sink.threshold (beast::severities::Severity::kAll);
        beast::Journal journal {sink};
        TestHandler handler;
        auto s = make_Server (handler,
            thread.get_io_service(), journal);
        std::vector<Port> list;
        list.resize(1);
        list.back().port = testPort;
        list.back().ip = boost::asio::ip::address::from_string (
            "127.0.0.1");
        list.back().protocol.insert("http");
        s->ports (list);

        test_request();
        //test_keepalive();
        //s->close();
        s = nullptr;

        pass();
    }

    void stressTest()
    {
        struct NullHandler
        {
            bool
            onAccept (Session& session,
                boost::asio::ip::tcp::endpoint endpoint)
            {
                return true;
            }

            Handoff
            onHandoff (Session& session,
                std::unique_ptr <beast::asio::ssl_bundle>&& bundle,
                    http_request_type&& request,
                        boost::asio::ip::tcp::endpoint remote_address)
            {
                return Handoff{};
            }

            Handoff
            onHandoff (Session& session, boost::asio::ip::tcp::socket&& socket,
                http_request_type&& request,
                    boost::asio::ip::tcp::endpoint remote_address)
            {
                return Handoff{};
            }

            void
            onRequest (Session& session)
            {
            }

            void
            onWSMessage(std::shared_ptr<WSSession> session,
                std::vector<boost::asio::const_buffer> const& buffers)
            {
            }

            void
            onClose (Session& session,
                boost::system::error_code const&)
            {
            }

            void
            onStopped (Server& server)
            {
            }
        };

        NullHandler h;
        for(int i = 0; i < 1000; ++i)
        {
            TestThread thread;
            auto s = make_Server(h,
                thread.get_io_service(), {});
            std::vector<Port> list;
            list.resize(1);
            list.back().port = testPort;
            list.back().ip = boost::asio::ip::address::from_string (
                "127.0.0.1");
            list.back().protocol.insert("http");
            s->ports (list);
        }
    }

    void
    run()
    {
        basicTests();
        stressTest();
    }
};

BEAST_DEFINE_TESTSUITE(Server,http,mtchain);

} // test
} //

