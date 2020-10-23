//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_TEST_JTX_JTX_H_INCLUDED
#define MTCHAIN_TEST_JTX_JTX_H_INCLUDED

#include <test/jtx/requires.h>
#include <test/jtx/basic_prop.h>
#include <mtchain/json/json_value.h>
#include <mtchain/protocol/STTx.h>
#include <mtchain/protocol/TER.h>
#include <functional>
#include <memory>
#include <vector>

namespace mtchain {
namespace test {
namespace jtx {

class Env;

/** Execution context for applying a JSON transaction.
    This augments the transaction with various settings.
*/
struct JTx
{
    Json::Value jv;
    requires_t requires;
    boost::optional<TER> ter = tesSUCCESS;
    bool fill_fee = true;
    bool fill_seq = true;
    bool fill_sig = true;
    std::shared_ptr<STTx const> stx;
    std::function<void(Env&, JTx&)> signer;

    JTx() = default;
    JTx (JTx const&) = default;
    JTx& operator=(JTx const&) = default;
    JTx(JTx&&) = default;
    JTx& operator=(JTx&&) = default;

    JTx (Json::Value&& jv_)
        : jv(std::move(jv_))
    {
    }

    JTx (Json::Value const& jv_)
        : jv(jv_)
    {
    }

    template <class Key>
    Json::Value&
    operator[](Key const& key)
    {
        return jv[key];
    }

    /** Return a property if it exists

        @return nullptr if the Prop does not exist
    */
    /** @{ */
    template <class Prop>
    Prop*
    get()
    {
        for (auto& prop : props_.list)
        {
            if (auto test = dynamic_cast<
                    prop_type<Prop>*>(
                        prop.get()))
                return &test->t;
        }
        return nullptr;
    }

    template <class Prop>
    Prop const*
    get() const
    {
        for (auto& prop : props_.list)
        {
            if (auto test = dynamic_cast<
                    prop_type<Prop> const*>(
                        prop.get()))
                return &test->t;
        }
        return nullptr;
    }
    /** @} */

    /** Set a property
        If the property already exists,
        it is replaced.
    */
    /** @{ */
    void
    set(std::unique_ptr<basic_prop> p)
    {
        for (auto& prop : props_.list)
        {
            if (prop->assignable(p.get()))
            {
                prop = std::move(p);
                return;
            }
        }
        props_.list.emplace_back(std::move(p));
    }

    template <class Prop, class... Args>
    void
    set(Args&&... args)
    {
        set(std::make_unique<
            prop_type<Prop>>(
                std::forward <Args> (
                    args)...));
    }
    /** @} */

private:
    struct prop_list
    {
        prop_list() = default;

        prop_list(prop_list const& other)
        {
            for (auto const& prop : other.list)
                list.emplace_back(prop->clone());
        }

        prop_list& operator=(prop_list const& other)
        {
            if (this != &other)
            {
                list.clear();
                for (auto const& prop : other.list)
                    list.emplace_back(prop->clone());
            }
            return *this;
        }

        prop_list(prop_list&& src) = default;
        prop_list& operator=(prop_list&& src) = default;

        std::vector<std::unique_ptr<
            basic_prop>> list;
    };

    prop_list props_;
};

} // jtx
} // test
} //

#endif
