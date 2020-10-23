//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/nodestore/TestBase.h>
#include <mtchain/nodestore/DummyScheduler.h>
#include <mtchain/nodestore/Manager.h>
#include <mtchain/nodestore/impl/DecodedBlob.h>
#include <mtchain/nodestore/impl/EncodedBlob.h>

namespace mtchain {
namespace NodeStore {

// Tests predictable batches, and NodeObject blob encoding
//
class NodeStoreBasic_test : public TestBase
{
public:
    // Make sure predictable object generation works!
    void testBatches (std::uint64_t const seedValue)
    {
        testcase ("batch");

        auto batch1 = createPredictableBatch (
            numObjectsToTest, seedValue);

        auto batch2 = createPredictableBatch (
            numObjectsToTest, seedValue);

        BEAST_EXPECT(areBatchesEqual (batch1, batch2));

        auto batch3 = createPredictableBatch (
            numObjectsToTest, seedValue + 1);

        BEAST_EXPECT(! areBatchesEqual (batch1, batch3));
    }

    // Checks encoding/decoding blobs
    void testBlobs (std::uint64_t const seedValue)
    {
        testcase ("encoding");

        auto batch = createPredictableBatch (
            numObjectsToTest, seedValue);

        EncodedBlob encoded;
        for (int i = 0; i < batch.size (); ++i)
        {
            encoded.prepare (batch [i]);

            DecodedBlob decoded (encoded.getKey (), encoded.getData (), encoded.getSize ());

            BEAST_EXPECT(decoded.wasOk ());

            if (decoded.wasOk ())
            {
                std::shared_ptr<NodeObject> const object (decoded.createObject ());

                BEAST_EXPECT(isSame(batch[i], object));
            }
        }
    }

    void run ()
    {
        std::uint64_t const seedValue = 50;

        testBatches (seedValue);

        testBlobs (seedValue);
    }
};

BEAST_DEFINE_TESTSUITE(NodeStoreBasic,mtchain_core,mtchain);

}
}
