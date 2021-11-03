/*
 * Copyright (C) 2021 The ViaDuck Project
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

#ifndef SECUREMEMORY_SPLITMIX64_H
#define SECUREMEMORY_SPLITMIX64_H

#include <cstdint>
#include <cstring>
#include <cinttypes>

#include "SafeInt.h"

/**
 * Fast non-cryptographically secure PRNG based on
 * SplitMix http://gee.cs.oswego.edu/dl/papers/oopsla14.pdf
 *
 * Fulfills the std::uniform_random_bit_generator requirement
 */
class SplitMix64 {
public:
    using result_type = uint_fast64_t;
    using seed_type = uint64_t;

    explicit SplitMix64(uint64_t seed = 0) : mState(seed) { }

    /**
     * @return Random number between 0 and max uint64
     */
    SM_NO_SANITIZE result_type next() {
        result_type z = (mState += UINT64_C(0x9e3779b97f4a7c15));
        z = (z ^ (z >> 30u)) * UINT64_C(0xbf58476d1ce4e5b9);
        z = (z ^ (z >> 27u)) * UINT64_C(0x94d049bb133111eb);
        return z ^ (z >> 31u);
    }

    /**
     * Fills given buffer with random bytes
     *
     * @param data Buffer pointer
     * @param size Size of buffer
     */
    void nextBytes(uint8_t *data, uint32_t size) {
        uint32_t nblocks = size / sizeof(result_type), nbytes = size % sizeof(result_type);
        auto *out = reinterpret_cast<result_type *>(data);

        // fill T blocks
        for (uint32_t i = 0; i < nblocks; i++)
            out[i] = next();

        // fill remaining bytes
        auto val = next();
        std::memcpy(out + nblocks, &val, nbytes);
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::lowest(); }
    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
    result_type operator()() { return next(); }

protected:
    result_type mState;
};

#endif //SECUREMEMORY_SPLITMIX64_H
