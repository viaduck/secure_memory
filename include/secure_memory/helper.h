/*
 * Copyright (C) 2015-2022 The ViaDuck Project
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

inline bool comparisonHelper(const void *one, const void *two, uint32_t size) {
    auto *c1 = static_cast<const char *>(one), *c2 = static_cast<const char *>(two);

    // loop breaks on different character or if size() elements have been checked
    for (; size != 0 && *c1 == *c2; c1++, c2++, size--);

    // if they equal, iteration count equals size
    return size == 0;
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
