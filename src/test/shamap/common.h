//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_SHAMAP_TESTS_COMMON_H_INCLUDED
#define MTCHAIN_SHAMAP_TESTS_COMMON_H_INCLUDED

#include <BeastConfig.h>
#include <mtchain/basics/contract.h>
#include <mtchain/basics/chrono.h>
#include <mtchain/shamap/Family.h>
#include <mtchain/shamap/FullBelowCache.h>
#include <mtchain/shamap/TreeNodeCache.h>
#include <mtchain/shamap/SHAMap.h>
#include <mtchain/basics/StringUtilities.h>
#include <mtchain/nodestore/DummyScheduler.h>
#include <mtchain/nodestore/Manager.h>
#include <mtchain/beast/utility/Journal.h>
#include <mtchain/beast/clock/manual_clock.h>

namespace mtchain {
namespace tests {

class TestFamily : public Family
{
private:
    TestStopwatch clock_;
    NodeStore::DummyScheduler scheduler_;
    TreeNodeCache treecache_;
    FullBelowCache fullbelow_;
    std::unique_ptr<NodeStore::Database> db_;
    beast::Journal j_;

public:
    TestFamily (beast::Journal j)
        : treecache_ ("TreeNodeCache", 65536, 60, clock_, j)
        , fullbelow_ ("full_below", clock_)
        , j_ (j)
    {
        Section testSection;
        testSection.set("type", "memory");
        testSection.set("Path", "SHAMap_test");
        db_ = NodeStore::Manager::instance ().make_Database (
            "test", scheduler_, j, 1, testSection);
    }

    beast::manual_clock <std::chrono::steady_clock>
    clock()
    {
        return clock_;
    }

    beast::Journal const&
    journal() override
    {
        return j_;
    }

    FullBelowCache&
    fullbelow() override
    {
        return fullbelow_;
    }

    FullBelowCache const&
    fullbelow() const override
    {
        return fullbelow_;
    }

    TreeNodeCache&
    treecache() override
    {
        return treecache_;
    }

    TreeNodeCache const&
    treecache() const override
    {
        return treecache_;
    }

    NodeStore::Database&
    db() override
    {
        return *db_;
    }

    NodeStore::Database const&
    db() const override
    {
        return *db_;
    }

    void
    missing_node (std::uint32_t refNum) override
    {
        Throw<std::runtime_error> ("missing node");
    }

    void
    missing_node (uint256 const& refHash) override
    {
        Throw<std::runtime_error> ("missing node");
    }
};

} // tests
} //

#endif
