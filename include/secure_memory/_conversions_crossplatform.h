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

// "License": Public Domain
// I, Mathias Panzenböck, place this file hereby into the public domain. Use it at your own risk for whatever you like.
// In case there are jurisdictions that don't support putting things in the public domain you can also consider it to
// be "dual licensed" under the BSD, MIT and Apache licenses, if you want to. This code is trivial anyway. Consider it
// an example on how to get the endian conversion functions on different platforms.

#ifndef SECUREMEMORY_CROSSPLATFORM_H
#define SECUREMEMORY_CROSSPLATFORM_H

#include <cinttypes>

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
    #define __WINDOWS__
    #define __bswap_16(x) __builtin_bswap16(x);
    #define __bswap_32(x) __builtin_bswap32(x);
    #define __bswap_64(x) __builtin_bswap64(x);
#endif

#if defined(__linux__) || defined(__CYGWIN__)
    #include <byteswap.h>
    #include <endian.h>

    #ifdef __ANDROID__
        #define be16toh(x) htobe16(x)
        #define le16toh(x) htole16(x)
        #define be32toh(x) htobe32(x)
        #define le32toh(x) htole32(x)
        #define be64toh(x) htobe64(x)
        #define le64toh(x) htole64(x)

        #define __bswap_16(x) __builtin_bswap16(x);
        #define __bswap_32(x) __builtin_bswap32(x);
        #define __bswap_64(x) __builtin_bswap64(x);
    #endif

#elif defined(__APPLE__)
    #include <libkern/OSByteOrder.h>

    #define htobe16(x) OSSwapHostToBigInt16(x)
    #define htole16(x) OSSwapHostToLittleInt16(x)
    #define be16toh(x) OSSwapBigToHostInt16(x)
    #define le16toh(x) OSSwapLittleToHostInt16(x)

    #define htobe32(x) OSSwapHostToBigInt32(x)
    #define htole32(x) OSSwapHostToLittleInt32(x)
    #define be32toh(x) OSSwapBigToHostInt32(x)
    #define le32toh(x) OSSwapLittleToHostInt32(x)

    #define htobe64(x) OSSwapHostToBigInt64(x)
    #define htole64(x) OSSwapHostToLittleInt64(x)
    #define be64toh(x) OSSwapBigToHostInt64(x)
    #define le64toh(x) OSSwapLittleToHostInt64(x)

    #define __BYTE_ORDER    BYTE_ORDER
    #define __BIG_ENDIAN    BIG_ENDIAN
    #define __LITTLE_ENDIAN LITTLE_ENDIAN
    #define __PDP_ENDIAN    PDP_ENDIAN

#elif defined(__OpenBSD__)
    #include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
    #include <sys/endian.h>

    #define be16toh(x) betoh16(x)
    #define le16toh(x) letoh16(x)

    #define be32toh(x) betoh32(x)
    #define le32toh(x) letoh32(x)

    #define be64toh(x) betoh64(x)
    #define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)
    #include <winsock2.h>

    #if !defined(_MSC_VER)
        #include <sys/param.h>
    #endif

    #if BYTE_ORDER == LITTLE_ENDIAN
        #if WINVER < 0x0620
            #define ntohll(x) (static_cast<uint64_t>(be32toh(static_cast<uint32_t>(x))) << 32 | static_cast<uint64_t>((be32toh(static_cast<uint32_t>(x >> 32)))))
            #define htonll(x) ntohll(x)
        #endif

        #define htobe16(x) htons(x)
        #define htole16(x) (x)
        #define be16toh(x) ntohs(x)
        #define le16toh(x) (x)

        #define htobe32(x) htonl(x)
        #define htole32(x) (x)
        #define be32toh(x) ntohl(x)
        #define le32toh(x) (x)

        #define htobe64(x) htonll(x)
        #define htole64(x) (x)
        #define be64toh(x) ntohll(x)
        #define le64toh(x) (x)

    #elif BYTE_ORDER == BIG_ENDIAN
        /* that would be xbox 360 */

        #define htobe16(x) (x)
        #define htole16(x) __builtin_bswap16(x)
        #define be16toh(x) (x)
        #define le16toh(x) __builtin_bswap16(x)

        #define htobe32(x) (x)
        #define htole32(x) __builtin_bswap32(x)
        #define be32toh(x) (x)
        #define le32toh(x) __builtin_bswap32(x)

        #define htobe64(x) (x)
        #define htole64(x) __builtin_bswap64(x)
        #define be64toh(x) (x)
        #define le64toh(x) __builtin_bswap64(x)

    #else

        #error byte order not supported

    #endif

    #define __BYTE_ORDER    BYTE_ORDER
    #define __BIG_ENDIAN    BIG_ENDIAN
    #define __LITTLE_ENDIAN LITTLE_ENDIAN
    #define __PDP_ENDIAN    PDP_ENDIAN

#else

    #error platform not supported

#endif

#endif //SECUREMEMORY_CROSSPLATFORM_H
