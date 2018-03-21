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

#ifndef SECUREMEMORY_DEVNULL_H
#define SECUREMEMORY_DEVNULL_H

#include "Buffer.h"

/**
 * Subclass of Buffer which noops some operations, behaving like a /dev/null
 */
class DevNull : public Buffer {

public:
    BufferRangeConst append(const void *data, uint32_t len) override;
    BufferRangeConst append(const char *data, uint32_t len) override;
    BufferRangeConst append(const Buffer &other) override;
    BufferRangeConst append(const BufferRangeConst &range) override;

    BufferRangeConst write(const void *data, uint32_t len, uint32_t offset) override;
    BufferRangeConst write(const Buffer &other, uint32_t offset) override;
    BufferRangeConst write(const BufferRangeConst &other, uint32_t offset) override;

    void consume(uint32_t n) override;
    void use(uint32_t used) override;
};


#endif //SECUREMEMORY_DEVNULL_H
