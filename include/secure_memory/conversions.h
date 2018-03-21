/*
 * Copyright (C) 2015-2018 The ViaDuck Project
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

/*
 * network-byte-order <-> host-byte-order conversions
 * network byte order is big-endian, therefore we can use be*toh and htobe* methods
 */

#ifndef SECUREMEMORY_CONVERSIONS_H
#define SECUREMEMORY_CONVERSIONS_H

#include <algorithm>

extern "C" {
    #include "_conversions_crossplatform.h"
}

#if BYTE_ORDER == LITTLE_ENDIAN
    template <typename T>
    inline T hton(T v) {
        // generic hton
        const uint8_t bytes = sizeof(T);
        T ret;
        for (uint8_t i = 0; i < bytes; ++i)
            reinterpret_cast<uint8_t*>(&ret)[bytes-i-1] = ((reinterpret_cast<uint8_t*>(&v)[i]));

        return ret;
    }

    template <>
    inline uint8_t hton(uint8_t v) {
        return v;
    }
    template <>
    inline int8_t hton(int8_t v) {
        return v;
    }

    template <>
    inline uint16_t hton(uint16_t v) {
        return htobe16(v);
    }
    template <>
    inline int16_t hton(int16_t v) {
        return htobe16(v);
    }

    template <>
    inline uint32_t hton(uint32_t v) {
        return htobe32(v);
    }
    template <>
    inline int32_t hton(int32_t v) {
        return htobe32(v);
    }

    template <>
    inline uint64_t hton(uint64_t v) {
        return htobe64(v);
    }
    template <>
    inline int64_t hton(int64_t v) {
        return htobe64(v);
    }
#else
    template <typename T>
    inline T hton(T v) {
        // generic hton
        return v;
    }
#endif

template <typename T>
inline T ntoh(T v) {
    return hton(v);
}

#endif //SECUREMEMORY_CONVERSIONS_H
