//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <mtchain/unity/rocksdb.h>
#include <test/nodestore/TestBase.h>
#include <mtchain/nodestore/DummyScheduler.h>
#include <mtchain/nodestore/Manager.h>
#include <mtchain/beast/utility/temp_dir.h>
#include <algorithm>

namespace mtchain {
namespace NodeStore {

// Tests the Backend interface
//
class Backend_test : public TestBase
{
public:
    void testBackend (
        std::string const& type,
        std::uint64_t const seedValue,
        int numObjectsToTest = 2000)
    {
        DummyScheduler scheduler;

        testcase ("Backend type=" + type);

        Section params;
        beast::temp_dir tempDir;
        params.set ("type", type);
        params.set ("path", tempDir.path());

        beast::xor_shift_engine rng (seedValue);

        // Create a batch
        auto batch = createPredictableBatch (
            numObjectsToTest, rng());

        beast::Journal j;

        {
            // Open the backend
            std::unique_ptr <Backend> backend =
                Manager::instance().make_Backend (params, scheduler, j);

            // Write the batch
            storeBatch (*backend, batch);

            {
                // Read it back in
                Batch copy;
                fetchCopyOfBatch (*backend, &copy, batch);
                BEAST_EXPECT(areBatchesEqual (batch, copy));
            }

            {
                // Reorder and read the copy again
                std::shuffle (
                    batch.begin(),
                    batch.end(),
                    rng);
                Batch copy;
                fetchCopyOfBatch (*backend, &copy, batch);
                BEAST_EXPECT(areBatchesEqual (batch, copy));
            }
        }

        {
            // Re-open the backend
            std::unique_ptr <Backend> backend = Manager::instance().make_Backend (
                params, scheduler, j);

            // Read it back in
            Batch copy;
            fetchCopyOfBatch (*backend, &copy, batch);
            // Canonicalize the source and destination batches
            std::sort (batch.begin (), batch.end (), LessThan{});
            std::sort (copy.begin (), copy.end (), LessThan{});
            BEAST_EXPECT(areBatchesEqual (batch, copy));
        }
    }

    //--------------------------------------------------------------------------

    void run ()
    {
        std::uint64_t const seedValue = 50;

        testBackend ("nudb", seedValue);

    #if MTCHAIN_ROCKSDB_AVAILABLE
        testBackend ("rocksdb", seedValue);
    #endif

    #ifdef MTCHAIN_ENABLE_SQLITE_BACKEND_TESTS
        testBackend ("sqlite", seedValue);
    #endif
    }
};

BEAST_DEFINE_TESTSUITE(Backend,mtchain_core,mtchain);

}
}
