//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_BASIC_PROP_H_INCLUDED
#define MTCHAIN_TEST_JTX_BASIC_PROP_H_INCLUDED

namespace mtchain {
namespace test {
namespace jtx {

struct basic_prop
{
    virtual ~basic_prop() = default;
    virtual std::unique_ptr<
        basic_prop> clone() const = 0;
    virtual bool assignable(
        basic_prop const*) const = 0;
};

template <class T>
struct prop_type : basic_prop
{
    T t;

    template <class... Args>
    prop_type(Args&&... args)
        : t(std::forward <Args>(args)...)
    {
    }

    std::unique_ptr<
        basic_prop> clone() const override
    {
        return std::make_unique<
            prop_type<T>>(t);
    }

    bool assignable(
        basic_prop const* src) const override
    {
        return dynamic_cast<
            prop_type<T> const*>(
                src);
    }
};

} // jtx
} // test
} //

#endif
