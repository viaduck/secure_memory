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

#include <string>
#include <cstring>
#include <cmath>

#include <secure_memory/SafeInt.h>
#include <secure_memory/helper.h>
#include <secure_memory/String.h>
#include <secure_memory/BufferRange.h>

String::String() : Buffer() { }

String::String(const char *c_str) : Buffer(c_str, strlen_s(c_str)) { }

String::String(const String &other) : Buffer(other.const_data(), other.size()) { }

String::String(const Buffer &other) : Buffer(other.const_data(), other.size()) { }

String String::operator+(const String &other) const {
    return concatHelper(other.const_data(), other.size());
}

String String::operator+(const char *c_str) const {
    return concatHelper(c_str, strlen_s(c_str));
}

String String::operator+(const std::string &stl_str) const {
    return concatHelper(stl_str.data(), stl_str.size());
}

String &String::operator+=(const String &other) {
    append(other.const_data(), other.size());
    return *this;
}

String &String::operator+=(const char *c_str) {
    append(c_str, strlen_s(c_str));
    return *this;
}

String &String::operator+=(const std::string &stl_str) {
    append(stl_str.c_str(), stl_str.size());
    return *this;
}

String &String::operator=(const String &other) {
    clear();
    append(other.const_data(), other.size());

    return *this;
}

bool String::operator==(const char *other) const {
    // without this check, crashes can occur if operator== is used incorrectly
    if (other == nullptr)
        return false;

    auto sz = strlen_s(other);
    return sz == size() && comparisonHelper(const_data(), other, sz);
}

bool String::operator==(const std::string &other) const {
    return size() == other.size() && comparisonHelper(const_data(), other.c_str(), size());
}

const char *String::c_str() {
    // we need to append a 0-termination char to the string, since it's stored without it internally
    auto range = mCStrings.append(const_data(), size());
    mCStrings.appendValue(0);

    return range.const_data<char>();
}

std::string String::stl_str() const {
    return {const_data<char>(), size()};
}

bool String::toInt(uint8_t base, uint32_t &result) const {
    static constexpr const char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
    static constexpr uint8_t gap = 'a' - '9' - 1;
    SafeInt<uint32_t> r;
    uint32_t exp = 0;

    // traverse array from back to front
    for (uint32_t i = size(); i > 0; i--) {
        auto digit = at<char>(i - 1);

        // digit must be in alphabet range, not in gap range
        if ((digit >= alphabet[0] && digit <= alphabet[std::min<uint8_t>(9, base - 1)])
                || (digit >= alphabet[10] && digit < alphabet[base])) {
            auto value = digit - alphabet[0] - (digit > alphabet[9] ? gap : 0);
            r += make_si<uint32_t>(static_cast<uint32_t>(value * pow(base, exp++)));
        }
    }

    result = r;
    return exp != 0;
}

String String::toHex(const uint8_t *data, uint32_t size) {
    constexpr const static char *alphabet = "0123456789abcdef";
    constexpr const uint8_t sixteen = 16;
    String result(make_si(size) * 2_si32);

    for (uint32_t i = 0; i < size && data; i++) {
        result.appendValue(alphabet[data[i] / sixteen]);
        result.appendValue(alphabet[data[i] % sixteen]);
    }

    return result;
}

/* PRIVATE */
String String::concatHelper(const void *c_str, uint32_t size) const {
    String newString(*this);
    newString.append(c_str, size);
    return newString;
}
