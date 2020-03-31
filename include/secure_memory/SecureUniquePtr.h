/*
 * Copyright (C) 2015-2020 The ViaDuck Project
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

#ifndef SECUREMEMORY_SECUREUNIQUEPTR_H
#define SECUREMEMORY_SECUREUNIQUEPTR_H

#include <memory>
#include <iostream>
#include <random>
#include <chrono>

class SecureUniquePtrPRNG {
public:
    /**
     * memset(..) with will not be optimized away by compilers (hopefully) used for memory erasing.
     */
    static void shred(void *dst, size_t len);

private:
    /**
     * PRNG for secure overwrite. The PRNG doesn't have to be cryptographic because the only reason why we
     * are overwriting with "random" values instead of a fixed value is obscurity: Compound memory blocks cannot be easily
     * determined.
     *
     * Thread safety is accomplished by thread_local keyword.
     */
    static thread_local std::mt19937 mRandGenerator;

    /**
     * @see SecureUniquePtr::mRandGenerator
     */
    static thread_local std::uniform_int_distribution<uint8_t> mRandDistribution;

    /**
     * Generates a PRNG value
     * @return PRNG value
     */
    static uint8_t get() {
        return mRandDistribution(mRandGenerator);
    }
};

/**
 * Wrapper around std::unique_ptr<T> which features secure memory erasing
 */
template<typename T>
class SecureUniquePtr {
public:
    /**
     * Creates a std::unique_ptr<T>
     */
    SecureUniquePtr() : ptr(new T()) { }

    /**
     * Move constructor. Other SecureUniquePtr will be reset.
     * @param other
     */
    SecureUniquePtr(SecureUniquePtr &&other) : ptr(std::move(other.ptr)) { }

    /**
     * Securely overwrite memory used by std::unique_ptr<T>
     */
    ~SecureUniquePtr() {
#ifdef OPTION_SECURE_UNIQUEPTR
        SecureUniquePtrPRNG::shred(ptr.get(), sizeof(T));
#endif
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T>
     * @return Internal std::unique_ptr<T>
     */
    std::unique_ptr<T, std::default_delete<T>> &operator()() {
        return ptr;
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T> (const)
     * @return Internal std::unique_ptr<T> (const)
     */
    const std::unique_ptr<T, std::default_delete<T>> &operator()() const {
        return ptr;
    }

    /**
     * Transfers ownership of other's std::unique_ptr<T> to us
     * @param other
     */
    SecureUniquePtr<T> &operator=(SecureUniquePtr<T> &&other) {
        ptr = std::move(other.ptr);

        return *this;
    }

private:
    std::unique_ptr<T, std::default_delete<T>> ptr;
};

/**
 * Wrapper around std::unique_ptr<T[]> which features secure memory erasing. This is the array version.
 */
template<typename T>
class SecureUniquePtr<T[]> {
public:
    /**
     * Creates a std::unique_ptr<T[]> with size elements.
     * @param size
     */
    SecureUniquePtr(size_t size) : mSize(size), ptr(new T[size]) { }

    /**
     * Transfers ownership of other's std::unique_ptr<T[]> to us
     * @param other
     */
    SecureUniquePtr(SecureUniquePtr<T[]> && other) {
        ptr = std::move(other.ptr);
        mSize = other.mSize;

        other.mSize = 0;
    }

    /**
     * Securely overwrite memory used by std::unique_ptr<T[]>
     */
    ~SecureUniquePtr() {
#ifdef OPTION_SECURE_UNIQUEPTR
        SecureUniquePtrPRNG::shred(ptr.get(), sizeof(T) * mSize);
#endif
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T[]>
     * @return Internal std::unique_ptr<T[]>
     */
    std::unique_ptr<T[], std::default_delete<T[]>> &operator()() {
        return ptr;
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T> (const)
     * @return Internal std::unique_ptr<T> (const)
     */
    const std::unique_ptr<T[], std::default_delete<T[]>> &operator()() const {
        return ptr;
    }

    /**
     * Transfers ownership of other's std::unique_ptr<T[]> to us
     * @param other
     */
    SecureUniquePtr<T[]> &operator=(SecureUniquePtr<T[]> &&other) {
        ptr = std::move(other.ptr);
        mSize = other.mSize;

        other.mSize = 0;
        return *this;
    }

    /**
     * Getter: array's number of elements
     */
    size_t size() const {
        return mSize;
    }

private:
    size_t mSize;
    std::unique_ptr<T[], std::default_delete<T[]>> ptr;
};

#endif //SECUREMEMORY_SECUREUNIQUEPTR_H
