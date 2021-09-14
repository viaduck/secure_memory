/*
 * Copyright (C) 2015-2021 The ViaDuck Project
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
#include <chrono>

#include <secure_memory/SplitMix64.h>

class MemoryShredder {
public:
    static void shred(void *data, size_t len);

private:
    static thread_local SplitMix64 sRng;
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
    SecureUniquePtr() : mPtr(new T()) { }

    /**
     * Move constructor. Other SecureUniquePtr will be reset.
     * @param other
     */
    SecureUniquePtr(SecureUniquePtr &&other) noexcept : mPtr(std::move(other.ptr)) { }

    /**
     * Securely overwrite memory used by std::unique_ptr<T>
     */
    ~SecureUniquePtr() {
        MemoryShredder::shred(mPtr.get(), sizeof(T));
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T>
     * @return Internal std::unique_ptr<T>
     */
    std::unique_ptr<T, std::default_delete<T>> &operator()() {
        return mPtr;
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T> (const)
     * @return Internal std::unique_ptr<T> (const)
     */
    const std::unique_ptr<T, std::default_delete<T>> &operator()() const {
        return mPtr;
    }

    /**
     * Transfers ownership of other's std::unique_ptr<T> to us
     * @param other
     */
    SecureUniquePtr<T> &operator=(SecureUniquePtr<T> &&other) noexcept {
        mPtr = std::move(other.ptr);

        return *this;
    }

private:
    std::unique_ptr<T, std::default_delete<T>> mPtr;
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
    explicit SecureUniquePtr(size_t size) : mPtr(new T[size]), mSize(size) { }

    /**
     * Transfers ownership of other's std::unique_ptr<T[]> to us
     * @param other
     */
    SecureUniquePtr(SecureUniquePtr<T[]> && other) noexcept : mPtr(std::move(other.mPtr)), mSize(other.mSize) {
        other.mSize = 0;
    }

    /**
     * Securely overwrite memory used by std::unique_ptr<T[]>
     */
    ~SecureUniquePtr() {
        MemoryShredder::shred(mPtr.get(), sizeof(T) * mSize);
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T[]>
     * @return Internal std::unique_ptr<T[]>
     */
    std::unique_ptr<T[], std::default_delete<T[]>> &operator()() {
        return mPtr;
    }

    /**
     * Convenience method for getting internal std::unique_ptr<T> (const)
     * @return Internal std::unique_ptr<T> (const)
     */
    const std::unique_ptr<T[], std::default_delete<T[]>> &operator()() const {
        return mPtr;
    }

    /**
     * Transfers ownership of other's std::unique_ptr<T[]> to us
     * @param other
     */
    SecureUniquePtr<T[]> &operator=(SecureUniquePtr<T[]> &&other) noexcept {
        mPtr = std::move(other.mPtr);
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
    std::unique_ptr<T[], std::default_delete<T[]>> mPtr;
    size_t mSize;
};

#endif //SECUREMEMORY_SECUREUNIQUEPTR_H
