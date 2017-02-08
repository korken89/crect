

#pragma once

#include "brigand/brigand.hpp"

namespace rtfm
{
namespace util
{

template <typename... T>
struct always_false : brigand::integral_constant<bool, false>
{
};

} /* END namespace details */
} /* END namespace rtfm */
