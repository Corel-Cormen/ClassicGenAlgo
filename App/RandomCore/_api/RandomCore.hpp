#pragma once

#include <limits>
#include <type_traits>

#include <QRandomGenerator>

class RandomCore
{
public:

    void init(quint32 seed = 0);

    template<typename T>
    T randUnit() const
    {
        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(generator.bounded(0, 2));
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            T value = static_cast<T>(generator.generateDouble());
            if (value != static_cast<T>(0.0))
            {
                value = std::min(value + std::numeric_limits<T>::epsilon(), static_cast<T>(1.0));
            }
            return value;
        }
        else
        {
            static_assert(std::is_arithmetic_v<T>, "Rand core required number type");
        }
    }

    template<typename T>
    T randNumber(const T num1, const T num2) const
    {
        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(generator.bounded(num1, num2));
        }
        else
        {
            static_assert(std::is_arithmetic_v<T>, "Rand core required integral type");
        }
    }

    size_t randDistIndex(std::vector<qreal> dist) const;

private:
    mutable QRandomGenerator generator;
};
