//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#ifndef MTCHAIN_NODESTORE_BASE_H_INCLUDED
#define MTCHAIN_NODESTORE_BASE_H_INCLUDED

#include <mtchain/nodestore/Database.h>
#include <mtchain/basics/random.h>
#include <mtchain/basics/StringUtilities.h>
#include <mtchain/beast/unit_test.h>
#include <mtchain/beast/utility/rngfill.h>
#include <mtchain/beast/xor_shift_engine.h>
#include <boost/algorithm/string.hpp>
#include <iomanip>

namespace mtchain {
namespace NodeStore {

/** Binary function that satisfies the strict-weak-ordering requirement.

    This compares the hashes of both objects and returns true if
    the first hash is considered to go before the second.

    @see std::sort
*/
struct LessThan
{
    bool
    operator()(
        std::shared_ptr<NodeObject> const& lhs,
            std::shared_ptr<NodeObject> const& rhs) const noexcept
    {
        return lhs->getHash () < rhs->getHash ();
    }
};

/** Returns `true` if objects are identical. */
inline
bool isSame (std::shared_ptr<NodeObject> const& lhs,
    std::shared_ptr<NodeObject> const& rhs)
{
    return
        (lhs->getType() == rhs->getType()) &&
        (lhs->getHash() == rhs->getHash()) &&
        (lhs->getData() == rhs->getData());
}

// Some common code for the unit tests
//
class TestBase : public beast::unit_test::suite
{
public:
    // Tunable parameters
    //
    static std::size_t const minPayloadBytes = 1;
    static std::size_t const maxPayloadBytes = 2000;
    static int const numObjectsToTest = 2000;

public:
    // Create a predictable batch of objects
    static
    Batch createPredictableBatch(
        int numObjects, std::uint64_t seed)
    {
        Batch batch;
        batch.reserve (numObjects);

        beast::xor_shift_engine rng (seed);

        for (int i = 0; i < numObjects; ++i)
        {
            NodeObjectType type;

            switch (rand_int(rng, 3))
            {
            case 0: type = hotLEDGER; break;
            case 1: type = hotACCOUNT_NODE; break;
            case 2: type = hotTRANSACTION_NODE; break;
            case 3: type = hotUNKNOWN; break;
            }

            uint256 hash;
            beast::rngfill (hash.begin(), hash.size(), rng);

            Blob blob (
                rand_int(rng,
                    minPayloadBytes, maxPayloadBytes));
            beast::rngfill (blob.data(), blob.size(), rng);

            batch.push_back (
                NodeObject::createObject(
                    type, std::move(blob), hash));
        }

        return batch;
    }

    // Compare two batches for equality
    static bool areBatchesEqual (Batch const& lhs, Batch const& rhs)
    {
        bool result = true;

        if (lhs.size () == rhs.size ())
        {
            for (int i = 0; i < lhs.size (); ++i)
            {
                if (! isSame(lhs[i], rhs[i]))
                {
                    result = false;
                    break;
                }
            }
        }
        else
        {
            result = false;
        }

        return result;
    }

    // Store a batch in a backend
    void storeBatch (Backend& backend, Batch const& batch)
    {
        for (int i = 0; i < batch.size (); ++i)
        {
            backend.store (batch [i]);
        }
    }

    // Get a copy of a batch in a backend
    void fetchCopyOfBatch (Backend& backend, Batch* pCopy, Batch const& batch)
    {
        pCopy->clear ();
        pCopy->reserve (batch.size ());

        for (int i = 0; i < batch.size (); ++i)
        {
            std::shared_ptr<NodeObject> object;

            Status const status = backend.fetch (
                batch [i]->getHash ().cbegin (), &object);

            BEAST_EXPECT(status == ok);

            if (status == ok)
            {
                BEAST_EXPECT(object != nullptr);

                pCopy->push_back (object);
            }
        }
    }

    void fetchMissing(Backend& backend, Batch const& batch)
    {
        for (int i = 0; i < batch.size (); ++i)
        {
            std::shared_ptr<NodeObject> object;

            Status const status = backend.fetch (
                batch [i]->getHash ().cbegin (), &object);

            BEAST_EXPECT(status == notFound);
        }
    }

    // Store all objects in a batch
    static void storeBatch (Database& db, Batch const& batch)
    {
        for (int i = 0; i < batch.size (); ++i)
        {
            std::shared_ptr<NodeObject> const object (batch [i]);

            Blob data (object->getData ());

            db.store (object->getType (),
                      std::move (data),
                      object->getHash ());
        }
    }

    // Fetch all the hashes in one batch, into another batch.
    static void fetchCopyOfBatch (Database& db,
                                  Batch* pCopy,
                                  Batch const& batch)
    {
        pCopy->clear ();
        pCopy->reserve (batch.size ());

        for (int i = 0; i < batch.size (); ++i)
        {
            std::shared_ptr<NodeObject> object = db.fetch (batch [i]->getHash ());

            if (object != nullptr)
                pCopy->push_back (object);
        }
    }
};

}
}

#endif
