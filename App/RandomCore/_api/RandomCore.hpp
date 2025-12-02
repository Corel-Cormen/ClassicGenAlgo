#pragma once

#include <limits>
#include <optional>
#include <type_traits>

#include <QRandomGenerator>
#include <QPair>

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
        else if constexpr (std::is_floating_point_v<T>)
        {
            return static_cast<T>(num1 + (num2 - num1) * generator.generateDouble());
        }
        else
        {
            static_assert(std::is_arithmetic_v<T>, "Rand core required number type");
        }
    }

    template<typename T>
    std::optional<QPair<T, T>> randTwo(const T num1, const T num2) const
    {
        if constexpr (std::is_integral_v<T>)
        {
            std::optional<QPair<T, T>> result;
            if (num2 > num1)
            {
                const size_t size = num2 - num1;
                if (size >= 2U)
                {
                    std::vector<T> numbers;
                    numbers.reserve(size);
                    for (auto i = num1; i <= num2; ++i)
                    {
                        numbers.push_back(i);
                    }

                    std::shuffle(numbers.begin(), numbers.end(), generator);

                    if (numbers[0] < numbers[1])
                    {
                        result = std::make_pair(numbers[0], numbers[1]);
                    }
                    else
                    {
                        result = std::make_pair(numbers[1], numbers[0]);
                    }
                }
            }
            return result;
        }
        else
        {
            static_assert(std::is_arithmetic_v<T>, "Rand core required integral type");
        }
    }

    size_t randDistIndex(std::vector<qreal> dist) const;

    qreal randNormal() const;

private:
    mutable QRandomGenerator generator;
};
