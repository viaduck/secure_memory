/*
 * Copyright (C) 2020-2021 The ViaDuck Project
 *
 * This file is part of SecureMemory.
 *
 * SecureMemory is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SecureMemory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with SecureMemory.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SECUREMEMORY_SAFEINT_H
#define SECUREMEMORY_SAFEINT_H

#include <limits>
#include <type_traits>

#ifdef __clang__
    #define SM_NO_SANITIZE __attribute__((no_sanitize("integer")))
#else
    #define SM_NO_SANITIZE
#endif

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
class SafeInt {
public:
    constexpr SafeInt() : mVal() {}
    constexpr explicit SafeInt(T val) : mVal(val) {}

    SafeInt<T> operator+(const SafeInt<T> &b) {
        bool decreasing = static_cast<T>(b) < static_cast<T>(0);
        T r;
        if (__builtin_add_overflow(mVal, static_cast<T>(b), &r))
            return SafeInt<T>(decreasing ? std::numeric_limits<T>::lowest() : std::numeric_limits<T>::max());
        return SafeInt<T>(r);
    }

    template <typename U>
    SafeInt<T> operator+(const U &b) = delete;

    SafeInt<T> &operator+=(const SafeInt<T> &b) {
        mVal = operator+(b);
        return *this;
    }

    SafeInt<T> operator-(const SafeInt<T> &b) {
        bool increasing = b < static_cast<T>(0);
        T r;
        if (__builtin_sub_overflow(mVal, static_cast<T>(b), &r))
            return SafeInt<T>(increasing ? std::numeric_limits<T>::max() : std::numeric_limits<T>::lowest());
        return SafeInt<T>(r);
    }

    template <typename U>
    SafeInt<T> operator-(const U &b) = delete;

    SafeInt<T> &operator-=(const SafeInt<T> &b) {
        mVal = operator-(b);
        return *this;
    }

    SafeInt<T> operator*(const SafeInt<T> &b) {
        bool negative = (b < static_cast<T>(0)) ^ (mVal < static_cast<T>(0));
        T r;
        if (__builtin_mul_overflow(mVal, static_cast<T>(b), &r))
            return SafeInt<T>(negative ? std::numeric_limits<T>::lowest() : std::numeric_limits<T>::max());
        return SafeInt<T>(r);
    }
    template <typename U>
    SafeInt<T> operator*(const U &b) = delete;

    SafeInt<T> operator*=(const SafeInt<T> &b) {
        return operator*(b);
    }

    template <typename U=T, typename = std::enable_if_t<std::is_signed<U>::value>>
    SafeInt<T> operator-() const {
        return SafeInt<T>(-mVal);
    }

    template <typename U=T, typename = std::enable_if_t<!std::is_signed<U>::value>, int = 0>
    SafeInt<T> operator-() const {
        static_assert(sizeof(T) == 0, "SafeInt: Unsafe unary operator.");
        return *this;
    }

    bool operator!=(const SafeInt<T> &rhs) const {
        return !(rhs == *this);
    }

    template<typename U>
    bool operator==(const SafeInt<U> &rhs) const {
        return mVal == rhs.mVal;
    }

    template<typename U>
    bool operator==(const U &rhs) const {
        return mVal == rhs;
    }

    operator T() const {
        return mVal;
    }

    template <typename U>
    SafeInt<T> &operator=(const U &rhs) {
        mVal = static_cast<T>(rhs);
        return *this;
    }

protected:
    T mVal;
};

template<typename T>
static SafeInt<T> make_si(T v) {
    return SafeInt<T>(v);
}

template<typename U, typename T>
bool operator==(const U &lhs, const SafeInt<T> &rhs) {
    return rhs == lhs;
}

constexpr SafeInt<unsigned long long int> operator "" _si(unsigned long long val) {
    return SafeInt<unsigned long long>(val);
}
constexpr SafeInt<long long int> operator "" _ssi(unsigned long long val) {
    return SafeInt<long long>(val);
}
constexpr SafeInt<uint8_t> operator "" _si8(unsigned long long val) {
    return SafeInt<uint8_t>(val);
}
constexpr SafeInt<int8_t> operator "" _ssi8(unsigned long long val) {
    return SafeInt<int8_t>(val);
}
constexpr SafeInt<uint16_t> operator "" _si16(unsigned long long val) {
    return SafeInt<uint16_t>(val);
}
constexpr SafeInt<int16_t> operator "" _ssi16(unsigned long long val) {
    return SafeInt<int16_t>(val);
}
constexpr SafeInt<uint32_t> operator "" _si32(unsigned long long val) {
    return SafeInt<uint32_t>(val);
}
constexpr SafeInt<int32_t> operator "" _ssi32(unsigned long long val) {
    return SafeInt<int32_t>(val);
}
constexpr SafeInt<uint64_t> operator "" _si64(unsigned long long val) {
    return SafeInt<uint64_t>(val);
}
constexpr SafeInt<int64_t> operator "" _ssi64(unsigned long long val) {
    return SafeInt<int64_t>(val);
}

#endif //SECUREMEMORY_SAFEINT_H
