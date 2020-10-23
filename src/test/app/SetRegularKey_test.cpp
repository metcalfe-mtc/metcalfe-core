//------------------------------------------------------------------------------
/*
    This file is part of mtchaind: https://github.com/MTChain/MTChain-core
    Copyright (c) 2017, 2018 MTChain Alliance.

    Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <mtchain/protocol/JsonFields.h>
#include <mtchain/protocol/Feature.h>
#include <test/jtx.h>

namespace mtchain {

class SetRegularKey_test : public beast::unit_test::suite
{
public:

    void testDisableMasterKey()
    {
        using namespace test::jtx;
        Env env(*this);
        Account const alice("alice");
        Account const bob("bob");
        env.fund(M(10000), alice, bob);

        // Master and Regular key
        env(regkey(alice, bob));
        auto const ar = env.le(alice);
        BEAST_EXPECT(ar->isFieldPresent(sfRegularKey) && (ar->getAccountID(sfRegularKey) == bob.id()));

        env(noop(alice));
        env(noop(alice), sig(bob));
        env(noop(alice), sig(alice));

        // Regular key only
        env(fset(alice, asfDisableMaster), sig(alice));
        env(noop(alice));
        env(noop(alice), sig(bob));
        env(noop(alice), sig(alice), ter(tefMASTER_DISABLED));
        env(fclear(alice, asfDisableMaster), sig(alice), ter(tefMASTER_DISABLED));
        env(fclear(alice, asfDisableMaster), sig(bob));
        env(noop(alice), sig(alice));
    }

    void testPasswordSpent()
    {
        using namespace test::jtx;
        Env env(*this);
        Account const alice("alice");
        Account const bob("bob");
        env.fund(M(10000), alice, bob);

        auto ar = env.le(alice);
        BEAST_EXPECT(ar->isFieldPresent(sfFlags) && ((ar->getFieldU32(sfFlags) & lsfPasswordSpent) == 0));

        env(regkey(alice, bob), sig(alice), fee(0));

        ar = env.le(alice);
        BEAST_EXPECT(ar->isFieldPresent(sfFlags) && ((ar->getFieldU32(sfFlags) & lsfPasswordSpent) == lsfPasswordSpent));

        // The second SetRegularKey transaction with Fee=0 should fail.
        env(regkey(alice, bob), sig(alice), fee(0), ter(telINSUF_FEE_P));

        env.trust(bob["USD"](1), alice);
        env(pay(bob, alice, bob["USD"](1)));
        ar = env.le(alice);
        BEAST_EXPECT(ar->isFieldPresent(sfFlags) && ((ar->getFieldU32(sfFlags) & lsfPasswordSpent) == 0));
    }

    void testUniversalMaskError()
    {
        using namespace test::jtx;
        Env env(*this);
        Account const alice("alice");
        Account const bob("bob");
        env.fund(M(10000), alice, bob);

        auto jv = regkey(alice, bob);
        jv[sfFlags.fieldName] = tfUniversalMask;
        env(jv, ter(temINVALID_FLAG));
    }

    void run()
    {
        testDisableMasterKey();
        testPasswordSpent();
        testUniversalMaskError();
    }

};

BEAST_DEFINE_TESTSUITE(SetRegularKey,app,mtchain);

}

