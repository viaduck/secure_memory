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

#ifndef SECUREMEMORY_BUFFER_H
#define SECUREMEMORY_BUFFER_H

#include "ISerializable.h"
#include "SecureUniquePtr.h"
#include "Range.h"
#include "SafeInt.h"

class Buffer;
using BufferRange = Range<Buffer>;
using BufferRangeConst = Range<const Buffer>;

class Buffer : public ISerializable {
public:
    /**
     * Creates a Buffer object with an internal buffer of reserved size.
     *
     * @param reserved Initial buffer capacity in bytes. Defaults to 512
     */
    explicit Buffer(uint32_t reserved = 512);
    /**
     * Creates a Buffer object from a byte sequence, copying it's contents
     *
     * @param bytes Byte sequence
     * @param size The size in bytes
     */
    explicit Buffer(const void *data, uint32_t size);
    /**
     * Creates a Buffer object from a BufferRangeConst
     *
     * @param range The buffer range containing the data to copy
     */
    explicit Buffer(const BufferRangeConst &range);
    /**
     * Creates a Buffer object from another Buffer (deep-copy).
     *
     * @param buffer A reference to the buffer to be copied
     */
    Buffer(const Buffer &buffer);
    /**
     * Move constructor. Other Buffer will be left in default state.
     *
     * @param buffer Other buffer's rvalue reference
     */
    Buffer(Buffer &&buffer) noexcept;

    /**
     * Destructor
     */
    virtual ~Buffer();

    /**
     * Appends a bunch of data to the Buffer; increases it's capacity if necessary.
     *
     * Capacity is at least double the capacity before.
     * @param data Data pointer
     * @param len Length of data (in bytes)
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst append(const void *data, uint32_t len);
    /**
     * Overload variant of append which appends the contents of another buffer to this buffer.
     *
     * @param other Other Buffer
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst append(const Buffer &other);
    /**
     * Overload variant of append which appends the contents of another buffer described by a Range to this buffer.
     *
     * @param other Other Buffer
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst append(const BufferRangeConst &range);
    /**
     * Variant of append which appends a single fundamental value for convenience.
     *
     * @tparam T type of value to append, must be fundamental
     * @param data Data value to append
     * @return Range containing information about added range within Buffer
     */
    template<typename T = uint8_t, typename std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
    BufferRangeConst appendValue(T data) {
        return append(&data, sizeof(T));
    }

    /**
     * Writes a bunch of data to the Buffer starting at specified offset;
     * overwriting existing data and increasing Buffer's capacity if necessary.
     *
     * @param data Data pointer (if nullptr: only allocate (if necessary) and use memory)
     * @param len Length of data (in bytes)
     * @param offset Starting position
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst write(const void *data, uint32_t len, uint32_t offset = 0);
    /**
     * Overload variant of write which writes the contents of another buffer to this buffer.
     *
     * @param other Other Buffer
     * @param offset Starting position
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst write(const Buffer &other, uint32_t offset = 0);
    /**
     * Overload variant of write which writes the contents of another buffer described by a Range to this buffer.
     *
     * @param other Other Buffer
     * @param offset Starting position
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst write(const BufferRange &other, uint32_t offset = 0);
    /**
     * Overload variant of write which writes the contents of another buffer described by a Range to this buffer.
     *
     * @param other Other Buffer
     * @param offset Starting position
     * @return Range containing information about added range within Buffer
     */
    BufferRangeConst write(const BufferRangeConst &other, uint32_t offset = 0);

    /**
     * Consumes n bytes from the beginning, moving the Buffer's beginning and shrinking the size;
     * Consumed bytes are discarded and might be overwritten by any subsequent operations.
     *
     * @param n The number of bytes to consume
     */
    void consume(uint32_t n);

    /**
     * Reduces consumed bytes count, increasing the size.
     *
     * @param n Number of bytes to un-consume
     */
    void unconsume(uint32_t n = 0);

    /**
     * Marks n bytes used at the end of the buffer. This increases the size.
     *
     * @param n Number of bytes to use
     */
    void use(uint32_t n);

    /**
     * Reduces used bytes from the end, shrinking the size.
     *
     * @param n Number of bytes to un-use
     */
    void unuse(uint32_t n);

    /**
     * Increases buffer capacity to newCapacity if necessary.
     *
     * @param newCapacity New capacity in bytes
     * @param by True if newCapacity is relative to size() or false if not (default)
     * @return New capacity
     */
    uint32_t increase(uint32_t newCapacity, bool by = false);
    /**
     * Overload variant of increase which initializes newly allocated memory to value.
     *
     * @param value Byte value of newly allocated (free) memory
     * @param by True if newCapacity is relative to size() or false if not (default)
     * @return New capacity
     */
    uint32_t increase(uint32_t newCapacity, uint8_t value, bool by = false);

    /**
     * Adds padded bytes with specified value to the Buffer (starting at offset), so that Buffer is newSize long. Padded
     * bytes are marked as used. Does not overwrite any existing bytes.
     *
     * @param offset Offset within Buffer
     * @param size Number of bytes to add
     * @param value Byte value of padded bytes
     */
    void padd(uint32_t offset, uint32_t size, uint8_t value);
    /**
     * Overload variant of padd which starts at end.
     *
     * @param newSize Buffer's new size
     * @param value Byte value of padded bytes
     */
    void padd(uint32_t newSize, uint8_t value);
    /**
     * Overload variant of padd which accepts a Range.
     *
     * @param range Range to padd the Buffer. The assigned object is not checked.
     * @param value Byte value of padded bytes.
     */
     void padd(BufferRange range, uint8_t value = 0);

    /**
     * Returns the Buffer's size.
     */
    uint32_t size() const;

    /**
     * Returns a constant data pointer to the buffer data at offset p.
     *
     * @param p Offset into used data to start at. Defaults to 0.
     */
    const void *const_data_raw(uint32_t p = 0) const;
    /**
     * Returns a typed constant data pointer to the buffer data at offset p.
     *
     * @tparam T Type of data pointed to by result.
     * @param p Offset into used data to start at. Defaults to 0.
     * @return Typed pointer into buffer.
     */
    template<typename T = uint8_t>
    const T *const_data(uint32_t p = 0) const {
        return static_cast<const T *>(const_data_raw(p));
    }
    /**
     * Returns a constant Range object from offset with size.
     *
     * @param offset The byte to start the Range from
     * @param size Range's size
     */
    BufferRangeConst const_data(uint32_t offset, uint32_t size) const;

    /**
     * Returns a mutable data pointer to the buffer data at offset p.
     *
     * @param p Offset into used data to start at. Defaults to 0.
     */
    void *data_raw(uint32_t p = 0);
    /**
     * Returns a typed mutable data pointer to the buffer data at offset p.
     *
     * @tparam T Type of data pointed to by result
     * @param p Offset into used data to start at. Defaults to 0.
     * @return Typed, mutable pointer into buffer.
     */
    template<typename T = uint8_t>
    T *data(uint32_t p = 0) {
        return static_cast<T *>(data_raw(p));
    }

    /**
     * Returns a mutable Range object from offset with size. Padds the region before returning.
     *
     * @param offset The byte to start the Range from
     * @param size Range's size
     */
    BufferRange data(uint32_t offset, uint32_t size);

    /**
     * Returns an instance of T from buffer data at offset p
     *
     * @param p Offset into used data to start at. Defaults to 0.
     * @return An instance of T
     */
    template<typename T = uint8_t>
    T at(uint32_t p = 0) const {
        return *const_data<T>(p);
    }

    /**
     * Create a resizable Range pointing to Buffer's end. Size is 0.
     *
     * @return Constructed Range
     */
    BufferRange end() {
        return {*this, size(), BufferRange::OBJ_END, true};
    }

    /**
     * Clears the buffer. This resets used and consumed bytes counts.
     *
     * @param shred Whether to overwrite all managed memory (slows down operation)
     */
    void clear(bool shred = false);

    /**
     * Compares two Buffers.
     *
     * This is a deep comparison, comparing the used bytes only, not taking into account Buffer's capacity.
     * @param other
     * @return True if contents of Buffers are the same
     */
    bool operator==(const Buffer &other) const;
    /**
     * Compares two Buffers.
     *
     * This is a deep comparison, comparing the used bytes only, not taking into account Buffer's capacity.
     * @param other
     * @return True if contents of Buffers differ
     */
    inline bool operator!=(const Buffer &other) const {
        return !operator==(other);
    }

    /**
     * Similar to move constructor. The other Buffer will be left in default state.
     *
     * @param other Other buffer's rvalue reference
     * @return *this
     * @see Buffer::Buffer(Buffer &&)
     */
    Buffer &operator=(Buffer &&other) noexcept;

    /**
     * Serializes this Buffer to the specified BufferRange.
     * Advances the specified Range by the amount of data written.
     *
     * @param out BufferRange where the serialized representation of this Buffer will be written to
     */
    void serialize(BufferRange &out) const;
    /**
     * Variant of serialize that serializes this Buffer to the specified BufferRange.
     * Does not advance the specified Range.
     *
     * @param out BufferRange where the serialized representation of this Buffer will be written to
     */
    void serializeTo(BufferRange out) const {
        serialize(out);
    }
    /**
     * Variant of serialize that serializes this Buffer to the given Buffer.
     * Appends the serialized representation of this Buffer to the specified Buffer.
     *
     * @param out Buffer to append serialization to
     */
    void serializeAppend(Buffer &out) const {
        serializeTo(out.end());
    }

    /**
     * Reads a serialized Buffer from given Range.
     * Advances the specified Range by the amount of data read.
     *
     * @param in Range pointing to a serialized representation of a Buffer
     * @return True on success, false otherwise
     */
    bool deserialize(BufferRangeConst &in);
    /**
     * Reads a serialized Buffer from given Range.
     * Does not advance the specified Range.
     *
     * @param in Range pointing to a serialized representation of a Buffer
     * @return True on success, false otherwise
     */
    bool deserializeFrom(BufferRangeConst in) {
        return deserialize(in);
    }

    /**
     * Implementation of Swappable concept, used for std::swap.
     *
     * @param one
     * @param two
     */
    friend void swap(Buffer &one, Buffer &two) {
        secure_memory::swap(one.mData, two.mData);
        secure_memory::swap(one.mReserved, two.mReserved);
        secure_memory::swap(one.mUsed, two.mUsed);
        secure_memory::swap(one.mOffset, two.mOffset);
    }

private:
    // internal data
    SecureUniquePtr<uint8_t[]> mData;
    // size of data, number of reserved bytes
    SafeInt<uint32_t> mReserved;
    // offset of used bytes in data
    SafeInt<uint32_t> mOffset {0};
    // used bytes in data, beginning at offset
    SafeInt<uint32_t> mUsed {0};
};

#endif //SECUREMEMORY_BUFFER_H
