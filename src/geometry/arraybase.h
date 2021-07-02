//
// Created by vvlla on 21.03.2021.
//

#ifndef RAYTRACER_ARRAYBASE_H
#define RAYTRACER_ARRAYBASE_H

#include <utility>

template<typename T, size_t size, size_t alignment = alignof(T)>
struct alignas(alignment) ArrayBase
{
public:
    constexpr ArrayBase() =default;

    explicit constexpr ArrayBase(const std::array<T, size>& arr) : arr(arr) {};

    template<typename... ArgTypes>
    constexpr ArrayBase(const T& first, ArgTypes&&... args) : arr{{first, static_cast<T>(std::forward<ArgTypes>(args))...}} {};

    using ArrayType = ArrayBase<T, size, alignment>;

    constexpr const T& operator[](size_t i) const
    {
        return arr[i];
    }

    constexpr T& operator[](size_t i)
    {
        return arr[i];
    }

    template<typename ArrT>
    constexpr ArrayType operator+(const ArrT& other) const
    {
        return ArrayType(*this) += other;
    }

    template<typename ArrT>
    constexpr ArrayType& operator+=(const ArrT& other)
    {
        for(size_t i = 0; i < size; ++i)
        {
            arr[i] += other[i];
        }
        return *this;
    }

    template<typename ArrT>
    constexpr ArrayType operator-(const ArrT& other) const
    {
        return ArrayType(*this) -= other;
    }

    template<typename ArrT>
    constexpr ArrayType& operator-=(const ArrT& other)
    {
        for(size_t i = 0; i < size; ++i)
        {
            arr[i] -= other[i];
        }
        return *this;
    }

protected:
    const ArrayType& Base() const
    {
        return *this;
    }

    ArrayType& Base()
    {
        return *this;
    }

    std::array<T, size> arr;
};


#endif //RAYTRACER_ARRAYBASE_H
