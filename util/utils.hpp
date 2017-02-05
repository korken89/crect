

#pragma once

#include "brigand/brigand.hpp"

template <typename... T>
struct always_false : brigand::integral_constant<bool, false>
{
};

