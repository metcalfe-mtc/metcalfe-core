//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2012-2017 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <test/jtx.h>
#include <test/jtx/Env.h>
#include <mtchain/protocol/JsonFields.h>

namespace mtchain {

class TransactionEntry_test : public beast::unit_test::suite
{
    void
    testBadInput()
    {
        testcase("Invalid request params");
        using namespace test::jtx;
        Env env {*this};

        {
            //no params
            auto const result = env.client()
                .invoke("transaction_entry", {})[jss::result];
            BEAST_EXPECT(result[jss::error] == "fieldNotFoundTransaction");
            BEAST_EXPECT(result[jss::status] == "error");
        }

        {
            Json::Value params {Json::objectValue};
            params[jss::ledger] = 20;
            auto const result = env.client()
                .invoke("transaction_entry", params)[jss::result];
            BEAST_EXPECT(result[jss::error] == "lgrNotFound");
            BEAST_EXPECT(result[jss::status] == "error");
        }

        {
            Json::Value params {Json::objectValue};
            params[jss::ledger] = "current";
            params[jss::tx_hash] = "DEADBEEF";
            auto const result = env.client()
                .invoke("transaction_entry", params)[jss::result];
            BEAST_EXPECT(result[jss::error] == "notYetImplemented");
            BEAST_EXPECT(result[jss::status] == "error");
        }

        {
            Json::Value params {Json::objectValue};
            params[jss::ledger] = "closed";
            params[jss::tx_hash] = "DEADBEEF";
            auto const result = env.client()
                .invoke("transaction_entry", params)[jss::result];
            BEAST_EXPECT(! result[jss::ledger_hash].asString().empty());
            BEAST_EXPECT(result[jss::error] == "transactionNotFound");
            BEAST_EXPECT(result[jss::status] == "error");
        }
    }

    void testRequest()
    {
        testcase("Basic request");
        using namespace test::jtx;
        Env env {*this};

        auto check_tx = [this, &env]
            (int index, std::string txhash, std::string type = "")
            {
                Json::Value resIndex, resHash;
                // first request using ledger_index to lookup
                {
                    Json::Value params {Json::objectValue};
                    params[jss::ledger_index] = index;
                    params[jss::tx_hash] = txhash;
                    resIndex = env.client()
                        .invoke("transaction_entry", params)[jss::result];
                    if(! BEAST_EXPECTS(resIndex.isMember(jss::tx_json), txhash))
                        return;
                    BEAST_EXPECT(resIndex[jss::tx_json][jss::hash] == txhash);
                    if(! type.empty())
                        BEAST_EXPECTS(
                            resIndex[jss::tx_json][jss::TransactionType] == type,
                            txhash + " is " +
                                resIndex[jss::tx_json][jss::TransactionType].asString());
                }

                // second request using ledger_hash to lookup and verify
                // both responses match
                {
                    Json::Value params {Json::objectValue};
                    params[jss::ledger_hash] = resIndex[jss::ledger_hash];
                    params[jss::tx_hash] = txhash;
                    resHash = env.client()
                        .invoke("transaction_entry", params)[jss::result];
                    BEAST_EXPECT(resHash == resIndex);
                }
            };

        Account A1 {"A1"};
        Account A2 {"A2"};

        env.fund(M(10000), A1);
        auto fund_1_tx =
            boost::lexical_cast<std::string>(env.tx()->getTransactionID());

        env.fund(M(10000), A2);
        auto fund_2_tx =
            boost::lexical_cast<std::string>(env.tx()->getTransactionID());

        env.close();

        // these are actually AccountSet txs because fund does two txs and
        // env.tx only reports the last one
        check_tx(env.closed()->seq(), fund_1_tx);
        check_tx(env.closed()->seq(), fund_2_tx);

        env.trust(A2["USD"](1000), A1);
        // the trust tx is actually a payment since the trust method
        // refunds fees with a payment after TrustSet..so just ignore the type
        // in the check below
        auto trust_tx =
            boost::lexical_cast<std::string>(env.tx()->getTransactionID());

        env(pay(A2, A1, A2["USD"](5)));
        auto pay_tx =
            boost::lexical_cast<std::string>(env.tx()->getTransactionID());
        env.close();

        check_tx(env.closed()->seq(), trust_tx);
        check_tx(env.closed()->seq(), pay_tx, "Payment");

        env(offer(A2, M(100), A2["USD"](1)));
        auto offer_tx =
            boost::lexical_cast<std::string>(env.tx()->getTransactionID());

        env.close();

        check_tx(env.closed()->seq(), offer_tx, "OfferCreate");
    }

public:
    void run ()
    {
        testBadInput();
        testRequest();
    }
};

BEAST_DEFINE_TESTSUITE (TransactionEntry, rpc, mtchain);

}  //