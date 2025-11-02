#pragma once

#include <cmath>
#include <type_traits>
#include <limits>

#include <QtTypes>

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
moreThan(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return (a + eps) > b;
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
lessEqThan(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return (a < b) || (std::fabs(a - b) <= eps);
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
moreEqThan(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return (a > b) || (std::fabs(a - b) <= eps);
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
equal(T a, T b, T eps = std::numeric_limits<T>::epsilon())
{
    return std::fabs(a - b) <= eps;
}

template <typename T1, typename T2>
T1 percentOf(const T1 num1, const T2 num2)
{
    return static_cast<T1>((static_cast<qreal>(num1) / static_cast<qreal>(num2)) * 100.0);
}

template <typename S, typename T1, typename T2>
S numberOfPercent(const T1 num1, const T2 num2)
{
    return static_cast<S>(static_cast<qreal>(num1) * (static_cast<qreal>(num2)/100.0));
}

} // end CommonFunc
