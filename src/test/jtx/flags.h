//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_FLAGS_H_INCLUDED
#define MTCHAIN_TEST_JTX_FLAGS_H_INCLUDED

#include <test/jtx/Env.h>
#include <mtchain/protocol/LedgerFormats.h>
#include <mtchain/protocol/TxFlags.h>
#include <mtchain/basics/contract.h>

namespace mtchain {
namespace test {
namespace jtx {

// JSON generators

/** Add and/or remove flag. */
Json::Value
fset (Account const& account,
    std::uint32_t on, std::uint32_t off = 0);

/** Remove account flag. */
inline
Json::Value
fclear (Account const& account,
    std::uint32_t off)
{
    return fset(account, 0, off);
}

namespace detail {

class flags_helper
{
protected:
    std::uint32_t mask_;

private:
    inline
    void
    set_args()
    {
    }

    void
    set_args (std::uint32_t flag)
    {
        switch(flag)
        {
        case asfRequireDest:    mask_ |= lsfRequireDestTag; break;
        case asfRequireAuth:    mask_ |= lsfRequireAuth; break;
        case asfDisallowM:    mask_ |= lsfDisallowM; break;
        case asfDisableMaster:  mask_ |= lsfDisableMaster; break;
        //case asfAccountTxnID: // ???
        case asfNoFreeze:       mask_ |= lsfNoFreeze; break;
        case asfGlobalFreeze:   mask_ |= lsfGlobalFreeze; break;
        case asfDefaultMtchain:  mask_ |= lsfDefaultMtchain; break;
        default:
        Throw<std::runtime_error> (
            "unknown flag");
        }
    }

    template <class Flag,
        class... Args>
    void
    set_args (std::uint32_t flag,
        Args... args)
    {
        set_args(flag, args...);
    }

protected:
    template <class... Args>
    flags_helper (Args... args)
        : mask_(0)
    {
        set_args(args...);
    }
};

} // detail

/** Match set account flags */
class flags : private detail::flags_helper
{
private:
    Account account_;

public:
    template <class... Args>
    flags (Account const& account,
            Args... args)
        : flags_helper(args...)
        , account_(account)
    {
    }

    void
    operator()(Env& env) const;
};

/** Match clear account flags */
class nflags : private detail::flags_helper
{
private:
    Account account_;

public:
    template <class... Args>
    nflags (Account const& account,
            Args... args)
        : flags_helper(args...)
        , account_(account)
    {
    }

    void
    operator()(Env& env) const;
};

} // jtx
} // test
} //

#endif
