/*
 * Copyright (C) 2015-2019 The ViaDuck Project
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

#include "secure_memory/SecureUniquePtr.h"

#if defined(__GNUC__) || !defined(__clang__)
#define SM_SUP_ASM_BARRIER 1
#endif

void SecureUniquePtrPRNG::shred(void *dst, size_t len) {
    if (dst == nullptr)
        return;

#ifdef SM_SUP_ASM_BARRIER
    for (char *buf = (char *) dst; len; buf[--len] = SecureUniquePtrPRNG::get());
    __asm__ __volatile__("" ::"r"(dst): "memory");
#else
    for (volatile char *buf = (volatile char *) dst; len; buf[--len] = SecureUniquePtrPRNG::get());
#endif
}

thread_local std::minstd_rand SecureUniquePtrPRNG::mRandGenerator(std::random_device().operator()());
thread_local std::uniform_int_distribution<uint8_t> SecureUniquePtrPRNG::mRandDistribution(std::numeric_limits<uint8_t>::min(),std::numeric_limits<uint8_t>::max());
