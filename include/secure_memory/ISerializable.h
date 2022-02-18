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

#ifndef SECUREMEMORY_ISERIALIZABLE_H
#define SECUREMEMORY_ISERIALIZABLE_H

/**
 * Interface indicating presence of some methods for data serialization. We don't use a pure virtual class because of
 * performance (vtable lookup).
 * TODO: Replace with C++ concepts once available (scheduled for c++20)
 */
class ISerializable {
    // uint32_t size() const;
    // void serialize(Buffer&) const;
    // bool deserialize(const Buffer&);
};

#endif //SECUREMEMORY_ISERIALIZABLE_H
