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

#include <random>

#include <secure_memory/SecureUniquePtr.h>

#if defined(__GNUC__) || !defined(__clang__)
#define SM_SUP_ASM_BARRIER 1
#endif

void MemoryShredder::shred(void *data, size_t size) {
    if (data == nullptr || size == 0)
        return;

#ifdef SECURE_MEMORY_UNIQUE_PTR_SHRED
    sRng.nextBytes(static_cast<uint8_t *>(data), size);
#else
    #warning "Disabled secure unique ptr deletion"
#endif

#ifdef SM_SUP_ASM_BARRIER
    __asm__ __volatile__(""::"r"(data): "memory");
#else
    volatile auto *buf = (volatile uint8_t *) data;
    buf[0] = buf[0];
#endif
}

thread_local SplitMix64 MemoryShredder::sRng(std::random_device().operator()());
