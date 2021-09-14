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

#include <string>
#include <cstring>
#include <cmath>

#include <secure_memory/SafeInt.h>
#include <secure_memory/helper.h>
#include <secure_memory/String.h>
#include <secure_memory/BufferRange.h>

String::String() : Buffer() { }

String::String(const char *cstring) : String(cstring, static_cast<uint32_t>(strlen_s(cstring))) { }

String::String(const char *cstring, uint32_t size) : Buffer(size) {
    Buffer::append(cstring, size);
}

String::String(const uint8_t *bytes, uint32_t size) : Buffer(size) {
    Buffer::append(bytes, size);
}

String::String(const String &other) : String(static_cast<const char*>(other.const_data()), other.size()) { }

String::String(const std::string &stlstring) : Buffer(static_cast<uint32_t>(stlstring.size())) {
    Buffer::append(stlstring.c_str(), static_cast<uint32_t>(stlstring.size()));
}

String::String(const Buffer &other) : String(static_cast<const uint8_t*>(other.const_data()), other.size()) { }

String String::operator+(const String &other) const {
    return concatHelper(static_cast<const char*>(other.const_data()), other.size());
}

String String::operator+(const char *cstring) const {
    return concatHelper(cstring, static_cast<uint32_t>(strlen_s(cstring)));
}

String String::operator+(const std::string &stlstring) const {
    return concatHelper(stlstring.c_str(), static_cast<uint32_t>(stlstring.size()));
}

String &String::operator+=(const String &other) {
    append(other.const_data(), other.size());
    return *this;
}

String &String::operator+=(const char *cstring) {
    append(cstring, static_cast<uint32_t>(strlen_s(cstring)));
    return *this;
}

String &String::operator+=(const std::string &stlstring) {
    append(stlstring.c_str(), static_cast<uint32_t>(stlstring.size()));
    return *this;
}

String &String::operator=(const String &other) {
    clear();
    append(other.const_data(), other.size());

    return *this;
}

bool String::operator==(const char *other) const {
    if (other == nullptr)       // without this check, there may occur crashes if == is wrongly used
        return false;

    auto cSize = static_cast<uint32_t>(strlen_s(other));
    if (size() != cSize)
        return false;
    return comparisonHelper(const_data(), other, cSize);
}

bool String::operator==(const std::string &other) const {
    if (size() != other.size())
        return false;
    return comparisonHelper(const_data(), other.c_str(), size());
}

const char *String::c_str() {
    // we need to append a 0-termination char to the string, since it's stored without it internally
    const void *p = mCStrings.append(const_data(), size()).const_data();
    mCStrings.append("", 1);

    return static_cast<const char *>(p);
}

std::string String::stl_str() const {
    return {static_cast<const char*>(const_data()), size()};
}

bool String::toInt(uint8_t base, uint32_t &result) const {
    SafeInt<uint32_t> r;
    result = 0;

    if (size() == 0)
        return false;

    constexpr const static char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
    constexpr const static uint8_t gap = 'a' - '9' - 1;     // difference between 9 and a since they are not siblings in ascii
    uint32_t exp = 0;

    // traverse array from back to front
    for (uint32_t i = size(); i != 0; --i) {
        uint8_t val = *(static_cast<const uint8_t*>(const_data(i-1)));
        if (val >= alphabet[0] && val < alphabet[base]) {        // within base bounds

            // need to apply an offset for alphabetic values
            uint32_t diff;
            if (val >= alphabet[10])
                diff = val - gap - alphabet[0];
            else
                diff = val - alphabet[0];

            r += make_si<uint32_t>(diff * pow(base, exp));
            exp++;
        }
    }

    result = r;
    return exp != 0;
}

String String::toHex(const uint8_t *data, uint32_t size) {
    if (data == nullptr || size == 0)
        return {};

    constexpr const static char *alphabet = "0123456789abcdef";
    constexpr const uint8_t sixteen = 16;

    SecureUniquePtr<char[]> final(make_si(size) * 2_si32 + 1_si32);
    final()[make_si(size) * 2_si32] = '\0';

    String s;
    uint8_t div, rem;

    for (uint32_t i = 0; i < size; ++i) {
        rem = data[i] % sixteen;
        final()[make_si(i) * 2_si32 + 1_si32] = alphabet[rem];

        div = data[i] / sixteen;
        final()[make_si(i) * 2_si32] = alphabet[div];
    }
    s += final().get();

    return s;
}

/* PRIVATE */
String String::concatHelper(const char *cstring, uint32_t size) const {
    String newString(*this);
    newString.append(cstring, size);
    return newString;
}
