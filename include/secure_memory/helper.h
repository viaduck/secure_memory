/*
 * Copyright (C) 2015-2023 The ViaDuck Project
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

#ifndef SECUREMEMORY_HELPER_H
#define SECUREMEMORY_HELPER_H

#include <cstring>
#include <functional>
#include "conversions.h"

inline bool comparisonHelper(const void *a, const void *b, uint32_t size) {
    auto *c1 = static_cast<const char *>(a), *c2 = static_cast<const char *>(b);

    // loop breaks on different character or if size() elements have been checked
    for (; size != 0 && *c1 == *c2; c1++, c2++, size--);

    // if they equal, iteration count equals size
    return size == 0;
}

inline bool lessHelper(const void *a, uint32_t sizeA, const void *b, uint32_t sizeB) {
    auto *u1 = static_cast<const uint8_t *>(a), *u2 = static_cast<const uint8_t *>(b);

    // loop breaks on different character or if sizeA or sizeB has been checked
    uint32_t i = 0;
    for(; i < sizeA && i < sizeB && u1[i] == u2[i]; i++);

    // if both sizes still fit, unequal byte was encountered, use operator<
    if (i < sizeA && i < sizeB)
        return u1[i] < u2[i];

    // if one size no longer fits, the shorter element is considered "less"
    return sizeA < sizeB;
}

inline size_t strlen_s(const char *str) {
    if (str == nullptr)
        return 0;
    return strlen(str);
}

namespace secure_memory {
    /**
     * Shortcut template to eliminate need for "using std::swap" before every swap call.
     * @param one
     * @param two
     */
    template<typename T>
    void swap(T &one, T &two) {
        using std::swap;
        swap(one, two);
    }
}

#endif //SECUREMEMORY_HELPER_H
