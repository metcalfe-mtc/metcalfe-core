//------------------------------------------------------------------------------
/*
  This file is part of mtchaind: https://github.com/MTChain/MTChain-core
  Copyright (c) 2012-2015 MTChain Alliance.

  Permission to use, copy, modify, and/or distribute this software for any

*/
//==============================================================================

#include <BeastConfig.h>
#include <test/jtx/Account.h>
#include <test/jtx/amount.h>
#include <cassert>
#include <cmath>
#include <iomanip>

namespace mtchain {
namespace test {
namespace jtx {

#if 0
std::ostream&
operator<<(std::ostream&& os,
    AnyAmount const& amount)
{
    if (amount.is_any)
    {
        os << amount.value.getText() << "/" <<
            to_string(amount.value.issue().currency) <<
                "*";
        return os;
    }
    os << amount.value.getText() << "/" <<
        to_string(amount.value.issue().currency) <<
            "(" << amount.name() << ")";
    return os;
}
#endif

PrettyAmount::operator AnyAmount() const
{
    return { amount_ };
}

template <typename T>
static
std::string
to_places(const T d, std::uint8_t places)
{
    assert(places <= std::numeric_limits<T>::digits10);

    std::ostringstream oss;
    oss << std::setprecision(places) << std::fixed << d;

    std::string out = oss.str();
    out.erase(out.find_last_not_of('0') + 1, std::string::npos);
    if (out.back() == '.')
        out.pop_back();

    return out;
}

std::ostream&
operator<< (std::ostream& os,
    PrettyAmount const& amount)
{
    if (amount.value().native())
    {
        // measure in hundredths
        auto const c =
            dropsPerM<int>::value / 100;
        auto const n = amount.value().mantissa();
        if(n < c)
        {
            if (amount.value().negative())
                os << "-" << n << " drops";
            else
                os << n << " drops";
            return os;
        }
        auto const d = double(n) /
            dropsPerM<int>::value;
        if (amount.value().negative())
            os << "-";

        os << to_places(d, 6) << " M";
    }
    else
    {
        os <<
            amount.value().getText() << "/" <<
                to_string(amount.value().issue().currency) <<
                    "(" << amount.name() << ")";
    }
    return os;
}

//------------------------------------------------------------------------------

M_t const M {};

PrettyAmount
IOU::operator()(epsilon_t) const
{
    return { STAmount(issue(), 1, -81),
        account.name() };
}

PrettyAmount
IOU::operator()(detail::epsilon_multiple m) const
{
    return { STAmount(issue(),
        static_cast<std::uint64_t>(m.n), -81),
            account.name() };
}

std::ostream&
operator<<(std::ostream& os,
    IOU const& iou)
{
    os <<
        to_string(iou.issue().currency) <<
            "(" << iou.account.name() << ")";
    return os;
}

any_t const any { };

} // jtx
} // test
} //
