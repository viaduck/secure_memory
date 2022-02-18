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

#ifndef SECUREMEMORY_RANGE_H
#define SECUREMEMORY_RANGE_H

#include <limits>

#include "SafeInt.h"
#include "helper.h"

template <typename O>
class Range {
public:
    /**
     * Creates a Range (offset, size) within an object.
     *
     * @param obj O object to construct the range on
     * @param offset Offset in Object to start at
     * @param size Range's size
     * @param resizable Defines if the Range is resizable
     */
    Range(O &obj, uint32_t offset, uint32_t size, bool resizable = false)
            : mObj(obj), mOffset(offset), mSize(size), mResizable(resizable) { }
    /**
     * Overload constructor with offset = 0 and size = OBJ_END
     *
     * @param obj O object to construct the range on
     */
    Range(O &obj) : Range(obj, 0, OBJ_END, true) { } // NOLINT(google-explicit-constructor)
    /**
     * Overload copy constructor for Range with different object type
     *
     * @tparam P Type of other Range object
     * @param other Other range to copy from
     */
    template<typename P>
    explicit Range(Range<P> &other)
            : Range(other.mObj, other.mOffset, other.mSize, other.mResizable) { }

    /**
     * @return Const object
     */
    inline O &const_object() const {
        return mObj;
    }
    /**
     * @return Mutable object
     */
    inline O &object() {
        return mObj;
    }

    /**
     * @return Size
     */
    inline uint32_t size() const {
        return (mSize == +OBJ_END ? mObj.size() - offset() : mSize);
    }
    /**
     * Set size
     */
    inline void size(uint32_t size) {
        mSize = size;
    }

    /**
     * @return Offset
     */
    inline uint32_t offset() const {
        return mOffset;
    }
    /**
     * Set offset
     */
    inline void offset(uint32_t offset) {
        mOffset = offset;
    }

    /**
     * @return Whether the Range is resizable
     */
    inline bool isResizable() const {
        return mResizable;
    }
    /**
     * Sets the resizable property
     *
     * @param resizable If the Range is resizable
     */
    inline void isResizable(bool resizable) {
        mResizable = resizable;
    }

    /**
     * Get typed pointer to underlying data at offset
     *
     * @tparam T Type of data pointed to by result
     * @param pos Offset (defaults to 0)
     */
    template<typename T = uint8_t>
    inline const T *const_data(uint32_t pos = 0) const {
        return const_object().template const_data<T>(make_si(pos) + mOffset);
    }
    /**
     * Get new constant Range for given offset and size relative to this Range's.
     * The new Range is not constrained to the space pointed to by this Range.
     *
     * @param off Starting offset for new Range, relative to this one.
     * @param sz Size of new Range
     * @return New constant Range on the same object with specified offset and size
     */
    Range<O> const_data(uint32_t off, uint32_t sz) const {
        return const_object().const_data(mOffset + make_si(off), sz);
    }

    /**
     * Get mutable, typed pointer to underlying data at offset
     *
     * @tparam T Type of data pointed to by result
     * @param pos Offset (defaults to 0)
     */
    template<typename T = uint8_t>
    inline T *data(uint32_t pos = 0) {
        return object().template data<T>(make_si(pos) + mOffset);
    }
    /**
     * Get new mutable Range for given offset and size relative to this Range's.
     * The new Range is not constrained to the space pointed to by this Range.
     *
     * @param off Starting offset for new Range, relative to this one.
     * @param sz Size of new Range
     * @return New mutable Range on the same object with specified offset and size
     */
    Range<O> data(uint32_t off, uint32_t sz) {
        return object().data(mOffset + make_si(off), sz);
    }

    /**
     * Write to the underlying object relative to this Range.
     *
     * @param data Pointer to data
     * @param size Size of data
     * @param off Offset relative to this Range's to write to in underlying object. Defaults to 0.
     */
    inline void write(const void *data, uint32_t size, uint32_t off = 0) {
        object().write(data, size, mOffset + make_si(off));
    }

    /**
     * Compares two Ranges
     *
     * @param other
     * @return True if content within the ranges is the same
     */
    bool operator==(const Range &other) const {
        return other.size() == size() && comparisonHelper(const_data(), other.const_data(), size());
    }
    /**
     * Compares two Ranges
     * @param other
     * @return True if content within the ranges is different
     */
    bool operator!=(const Range &other) const {
        return !(other == *this);
    }
    /**
     * Shrinks the Range by moving it forward.
     * Internally, this increases the offset and reduces the size.
     *
     * @param addition Amount of bytes to move
     * @return this
     */
    Range &operator+=(uint32_t addition) {
        if (addition > size())
            addition = size();

        mOffset += make_si(addition);
        mSize -= make_si(mSize != OBJ_END ? addition : 0);
        return *this;
    }

    /**
     * Tries to ensure the range has specified size (if possible).
     *
     * @param dest Range to padd
     * @param size Target size
     * @return True if Range has the required size or can be increased to size
     */
    bool ensureSize(uint32_t sz) {
        // not enough size and not resizable -> fail
        if (sz > size() && !isResizable())
            return false;

        // not enough size and resizable -> increase size (will be applied to buffer in padd)
        if (sz > size())
            size(sz);

        // the range in the buffer will be allocated and initialized to zero
        object().padd(*this);
        return true;
    }

    // constant for size sticking to the end
    const static uint32_t OBJ_END = std::numeric_limits<uint32_t>::max();

protected:
    // allow Ranges with different type protected access
    template<typename P>
    friend class Range;

    // object holding underlying range data
    O &mObj;
    // offset into object, start of range
    SafeInt<uint32_t> mOffset;
    // size of object range, starting at offset
    SafeInt<uint32_t> mSize;
    // whether the underlying object allows resizing the range
    bool mResizable = false;
};

#endif //SECUREMEMORY_RANGE_H
