//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/shamap/SHAMap.h>
#include <mtchain/shamap/SHAMapItem.h>
#include <test/shamap/common.h>
#include <mtchain/basics/random.h>
#include <mtchain/basics/StringUtilities.h>
#include <mtchain/beast/unit_test.h>

namespace mtchain {
namespace tests {

class sync_test : public beast::unit_test::suite
{
public:
    static std::shared_ptr<SHAMapItem> makeRandomAS ()
    {
        Serializer s;

        for (int d = 0; d < 3; ++d)
            s.add32 (rand_int<std::uint32_t>());

        return std::make_shared<SHAMapItem>(
            s.getSHA512Half(), s.peekData ());
    }

    bool confuseMap (SHAMap& map, int count)
    {
        // add a bunch of random states to a map, then remove them
        // map should be the same
        SHAMapHash beforeHash = map.getHash ();

        std::list<uint256> items;

        for (int i = 0; i < count; ++i)
        {
            std::shared_ptr<SHAMapItem> item = makeRandomAS ();
            items.push_back (item->key());

            if (!map.addItem (std::move(*item), false, false))
            {
                log << "Unable to add item to map\n";
                return false;
            }
        }

        for (auto const& item : items)
        {
            if (!map.delItem (item))
            {
                log << "Unable to remove item from map\n";
                return false;
            }
        }

        if (beforeHash != map.getHash ())
        {
            log <<
                "Hashes do not match " << beforeHash <<
                " " << map.getHash () << std::endl;
            return false;
        }

        return true;
    }

    void run()
    {
        log << "Run, version 1\n" << std::endl;
        run(SHAMap::version{1});

        log << "Run, version 2\n" << std::endl;
        run(SHAMap::version{2});
    }

    void run(SHAMap::version v)
    {
        beast::Journal const j; // debug journal
        TestFamily f(j), f2(j);
        SHAMap source (SHAMapType::FREE, f, v);
        SHAMap destination (SHAMapType::FREE, f2, v);

        int items = 10000;
        for (int i = 0; i < items; ++i)
        {
            source.addItem (std::move(*makeRandomAS ()), false, false);
            if (i % 100 == 0)
                source.invariants();
        }

        source.invariants();
        BEAST_EXPECT(confuseMap (source, 500));
        source.invariants();

        source.setImmutable ();

        int count = 0;
        source.visitLeaves([&count](auto const& item)
            {
                ++count;
            });
        BEAST_EXPECT(count == items);

        std::vector<SHAMapMissingNode> missingNodes;
        source.walkMap(missingNodes, 2048);
        BEAST_EXPECT(missingNodes.empty());

        std::vector<SHAMapNodeID> nodeIDs, gotNodeIDs;
        std::vector< Blob > gotNodes;
        std::vector<uint256> hashes;

        std::vector<SHAMapNodeID>::iterator nodeIDIterator;
        std::vector< Blob >::iterator rawNodeIterator;

        destination.setSynching ();

        {
            std::vector<SHAMapNodeID> gotNodeIDs_a;
            std::vector<Blob> gotNodes_a;

            BEAST_EXPECT(source.getNodeFat (
                SHAMapNodeID (),
                gotNodeIDs_a,
                gotNodes_a,
                rand_bool(),
                rand_int(2)));

            unexpected (gotNodes_a.size () < 1, "NodeSize");

            BEAST_EXPECT(destination.addRootNode (
                source.getHash(),
                makeSlice(*gotNodes_a.begin ()),
                snfWIRE,
                nullptr).isGood());
        }

        do
        {
            f.clock().advance(std::chrono::seconds(1));

            // get the list of nodes we know we need
            auto nodesMissing = destination.getMissingNodes (2048, nullptr);

            if (nodesMissing.empty ())
                break;

            // get as many nodes as possible based on this information
            std::vector<SHAMapNodeID> gotNodeIDs_b;
            std::vector<Blob> gotNodes_b;

            for (auto& it : nodesMissing)
            {
                BEAST_EXPECT(source.getNodeFat (
                    it.first,
                    gotNodeIDs_b,
                    gotNodes_b,
                    rand_bool(),
                    rand_int(2)));
            }

            BEAST_EXPECT(gotNodeIDs_b.size () == gotNodes_b.size ());
            BEAST_EXPECT(!gotNodeIDs_b.empty ());

            for (std::size_t i = 0; i < gotNodeIDs_b.size(); ++i)
            {
                BEAST_EXPECT(
                    destination.addKnownNode (
                        gotNodeIDs_b[i],
                        makeSlice(gotNodes_b[i]),
                        nullptr).isUseful ());
            }
        }
        while (true);

        destination.clearSynching ();

        BEAST_EXPECT(source.deepCompare (destination));

        log << "Checking destination invariants..." << std::endl;
        destination.invariants();
    }
};

BEAST_DEFINE_TESTSUITE(sync,shamap,mtchain);

} // tests
} //
