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

template <typename T>
T alignMinValue(const T value, const T min1, const T min2)
{
    T result = value;
    const T minVal = lessThan(min1, min2) ? min1 : min2;
    if (CommonFunc::lessThan(value, minVal))
    {
        result = minVal;
    }
    return result;
}

template <typename T>
T alignMaxValue(const T value, const T max1, const T max2)
{
    T result = value;
    const T maxVal = CommonFunc::lessThan(max1, max2) ? max1 : max2;
    if (CommonFunc::moreEqThan(value, maxVal))
    {
        result = maxVal;
    }
    return result;
}

template <typename T>
T calculateDistance(const T g1, const T g2)
{
    return std::abs(g1 - g2);
}

template <typename T>
T calcMinElement(const T g1, const T g2, const T alpha)
{
    return (std::min(g1, g2) - (alpha * calculateDistance(g1, g2)));
}

template <typename T>
T calcMaxElement(const T g1, const T g2, const T alpha)
{
    return (std::max(g1, g2) + (alpha * calculateDistance(g1, g2)));
}

template <typename T>
T alignValue(const T val, const T min1, const T min2, const T max1, const T max2)
{
    T result = val;

    if (CommonFunc::lessThan(val, min1) ||
        CommonFunc::lessThan(val, min2))
    {
        result = CommonFunc::alignMinValue(val, min1, min2);
    }
    if (CommonFunc::moreThan(val, max1) ||
        CommonFunc::moreThan(val, max2))
    {
        result = CommonFunc::alignMaxValue(val, min1, max2);
    }

    return result;
}

} // end CommonFunc
