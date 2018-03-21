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

#include <secure_memory/DevNull.h>
#include <secure_memory/Range.h>
#include <secure_memory/BufferRange.h>


BufferRangeConst DevNull::append(const void *data, uint32_t len) {
    return Buffer::append(data, len);
}

BufferRangeConst DevNull::append(const char *data, uint32_t len) {
    return Buffer::append(static_cast<const void *>(data), len);
}

BufferRangeConst DevNull::append(const Buffer &other) {
    return Buffer::append(other);
}

BufferRangeConst DevNull::append(const BufferRangeConst &range) {
    return Buffer::append(range);
}

void DevNull::consume(uint32_t /* n */) {

}

void DevNull::use(uint32_t /* used */) {

}

BufferRangeConst DevNull::write(const void */* data */, uint32_t len, uint32_t offset) {
    increase(offset+len);
    return BufferRangeConst(*this, offset, len);
}

BufferRangeConst DevNull::write(const Buffer &other, uint32_t offset) {
    return Buffer::write(other, offset);
}

BufferRangeConst DevNull::write(const BufferRangeConst &other, uint32_t offset) {
    return Buffer::write(other, offset);
}
