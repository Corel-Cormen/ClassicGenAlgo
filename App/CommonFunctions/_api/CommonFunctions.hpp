#pragma once

#include <cmath>
#include <type_traits>
#include <limits>

namespace CommonFunc
{

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
lessThan(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return (a + eps) < b;
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
lessEqThan(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return (a < b) || (std::fabs(a - b) <= eps);
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
equal(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return std::fabs(a - b) <= eps;
}

} // end CommonFunc
