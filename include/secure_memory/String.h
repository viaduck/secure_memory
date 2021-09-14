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

#ifndef SECUREMEMORY_STRING_H
#define SECUREMEMORY_STRING_H

#include <istream>

#include "Buffer.h"
#include "BufferRange.h"

class String : public Buffer {

public:
    /**
     * Creates an empty String
     */
    String();

    /**
     * Creates a String object from a c-style string or byte sequence, copying it's contents.
     *
     * Warning: The string must be 0-terminated!
     * @param cstring C-style string
     */
    String(const char *cstring);

    /**
     * Creates a String object from a c-style string or byte sequence, copying it's contents
     * @param cstring C-style string
     * @param size The c-style string's size in characters (excluding 0-termination)
     */
    String(const char *cstring, uint32_t size);

    /**
     * Creates a String object from a byte sequence, copying it's contents
     * @param bytes Byte sequence
     * @param size The c-style string's size in characters (excluding 0-termination)
     */
    String(const uint8_t *bytes, uint32_t size);

    /**
     * Creates a String object from an STL string (std::string), copying it's contents
     * @param stlstring The std::string object
     */
    String(const std::string &stlstring);

    /**
     * Creates a String object from another String, copying it's contents
     * @param other The other String object
     */
    String(const String &other);

    /**
     * Creates a String object from a Buffer, copying it's contents
     * @param other The other Buffer object
     */
    String(const Buffer &other);

    /**
     * Concatenates this and another String
     * @param other Other String to concatenate
     * @return A new String object containing the concatenated strings
     */
    String operator+(const String &other) const;
    /**
     * Concatenates this and a c-style string
     * @param other Other c-style string to concatenate
     * @return A new String object containing the concatenated strings
     */
    String operator+(const char *cstring) const;
    /**
     * Concatenates this and an STL string (std::string)
     * @param other Other std::string to concatenate
     * @return A new String object containing the concatenated strings
     */
    String operator+(const std::string &stlstring) const;

    /**
     * Concatenates this and another String
     * @param other Other String to concatenate
     * @return A new String object containing the concatenated strings
     */
    String &operator+=(const String &other);
    /**
     * Concatenates this and a c-style string
     * @param other Other c-style string to concatenate
     * @return A new String object containing the concatenated strings
     */
    String &operator+=(const char *cstring);
    /**
     * Concatenates this and an STL string (std::string)
     * @param other Other std::string to concatenate
     * @return A new String object containing the concatenated strings
     */
    String &operator+=(const std::string &stlstring);

    /**
     * Compares a String and a cstring (byte-comparison)
     * @param other Cstring to compare to this String (must be 0-terminated)
     * @return True if they are equal, false if not
     */
    bool operator==(const char *other) const;
    /**
     * Compares a String and an stl string (byte-comparison)
     * @param other STL string (std::string) to compare to this String
     * @return True if they are equal, false if not
     */
    bool operator==(const std::string &other) const;

    /**
     * Compares a String and a cstring (byte-comparison)
     * @param other Cstring to compare to this String (must be 0-terminated)
     * @return True if they are equal, false if not
     */
    inline bool operator!=(const char *other) const {
        return !operator==(other);
    }
    /**
     * Compares a String and an stl string (byte-comparison)
     * @param other STL string (std::string) to compare to this String
     * @return True if they are equal, false if not
     */
    inline bool operator!=(const std::string &other) const {
        return !operator==(other);
    }

    // also include comparators of Buffer
    using Buffer::operator==;
    using Buffer::operator!=;

    /**
     * Checks if String is empty
     * @return True if String is empty, false if not
     */
    inline bool isEmpty() const {
        return size() == 0;
    }

    /**
     * Assigns a new String, copying it's contents
     * @param other The other String object
     * @return Reference to this
     */
    String &operator=(const String &other);

    /**
     * Returns a pointer to a c-style representation (0-terminated) of this String.
     * The returned pointer will always hold the string data at invocation time. Modifications to String later on will NOT be reflected to pointer.
     *
     * Its lifetime is bound to the String's lifetime.
     * Warning: Do not alter the returned pointer's memory!
     *
     * @return C-style string
     */
    const char *c_str();

    /**
     * Returns an STL string representation (std::string) of this String
     * @return STL string
     */
    std::string stl_str() const;

    /**
     * Converts the String to a number
     * If conversion was unsuccessful, result will be 0.

     * LIMITATIONS: All signed numbers are treated as unsigned
     * @param base Conversion base
     * @param result The converted number
     * @return If conversion was successful
     */
    bool toInt(uint8_t base, uint32_t &result) const;

    /**
     * Creates a Hex string from the contained binary data
     * @param data Binary data
     * @param size Data size
     * @return New String instance containing the hex string
     */
    inline String toHex() const {
        return String::toHex(static_cast<const uint8_t *>(const_data()), size());
    }

    /**
     * Creates a Hex string from the specified binary data
     * @param data Binary data
     * @param size Data size
     * @return String instance containing the hex string
     */
    static String toHex(const uint8_t *data, uint32_t size);

    /**
     * Stream operator supporting e.g. streaming std::cin into String
     * @param is std::istream to read from
     * @param string String instance to write contents to
     * @return param is (for chaining)
     */
    friend std::istream &operator>>(std::istream &is, String &string) {
        string.increase(sizeof(char)*512);
        is.getline(static_cast<char *>(string.data()), sizeof(char)*512);

        // we don't want the istream 0-terminator
        string.use(static_cast<uint32_t>(is.gcount() > 0 ? is.gcount() - 1 : 0u));
        return is;
    }

    /**
     * Stream operator supporting e.g. streaming String into std::out
     * @param is std::ostream to write to
     * @param string String instance to read contents from
     * @return param os (for chaining)
     */
    friend std::ostream &operator<<(std::ostream &os, const String &string) {
        os.write(static_cast<const char *>(string.const_data()), string.size());
        return os;
    }

    /**
     * Compares two Strings lexically
     * // FIXME: this is just enough to use a set<String>, but does not actually compare lexically
     *
     * @param other String to compare to this String
     * @return True if this String is lexically smaller than the other one
     */
    inline virtual bool operator<(const String &other) const {
        if(other.size() != size() || size() == 0)
            return size() < other.size();

        const auto *s1 = static_cast<const uint8_t *>(const_data());
        const auto *s2 = static_cast<const uint8_t *>(other.const_data());

        uint32_t s_pos = 0;
        while(s1[s_pos] == s2[s_pos] && ++s_pos < size() - 1);

        return s1[s_pos] < s2[s_pos];
    }

    using Buffer::deserialize;
    using Buffer::serialize;

protected:
    Buffer mCStrings;

private:
    String concatHelper(const char *cstring, uint32_t size) const;
};

namespace std {

    /**
     * Implement hash function for String class (so that String is usable in a hash map)
     */
    template<>
    struct hash<const String> {
        std::size_t operator()(const String &k) const {
            std::hash<const BufferRangeConst> test;
            return test.operator()(k);
        }
    };
}


#endif //SECUREMEMORY_STRING_H
