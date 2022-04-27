/*
 * Copyright (C) 2022 The ViaDuck Project
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
#ifndef SECUREMEMORY_BASEN_H
#define SECUREMEMORY_BASEN_H

#include <string>
#include <numeric>
#include <cinttypes>

#include "BufferRange.h"
#include "String.h"

/**
 * Generic coder for any base (e.g. Base64, Base16, Hex). Various pre-defined coders exist. User-defined coders
 * for any alphabet can be created, but the alphabet must be known at compile time since coding tables are generated
 * during the compilation step.
 */
class BaseN {
protected:
    template <size_t Base>
    struct CodingTable {
        char paddingChar = 0;
        char encoding[Base] = {};
        char decoding[256] = {};

        const char invalidChar = Base + 1;
    };

    /**
     * Creates a coding table from a given alphabet and optional padding character.
     *
     * This function is constexpr.
     * @tparam Base N of Base
     * @tparam AlphabetSize Size of alphabet
     * @param alphabet Alphabet
     * @param paddingChar Padding character or 0 if unused
     * @return Generated coding table.
     */
    template<size_t Base, size_t AlphabetSize>
    static constexpr CodingTable<Base> createCodingTable(const char (&alphabet)[AlphabetSize], char paddingChar) {
        // AlphabetSize without string-terminator must be multiple of Base
        static_assert(((AlphabetSize - 1) % Base) == 0);
        CodingTable<Base> table { .paddingChar = paddingChar };

        // copy encoding characters: these are only the first Base amount of characters
        for (size_t i = 0; i < Base; i++)
            table.encoding[i] = alphabet[i];

        // set invalid decoding values
        for (auto &val : table.decoding)
            val = table.invalidChar;

        // decode padding character to 0 for correct calculations
        table.decoding[int(table.paddingChar)] = 0;

        // set valid decoding values: these include all possible character, not only the first Base amount
        for (size_t i = 0; i < AlphabetSize; i++)
            table.decoding[int(alphabet[i])] = i % Base;

        return table;
    }

public:
    /**
     * Class to be subclassed for user-defined Base coder.
     * @tparam BitsPerChar Bits per encoded character
     * @tparam Base N of Base
     * @tparam AlphabetSize Size of Alphabet
     * @tparam Alphabet Alphabet
     * @tparam PaddingChar Optional padding character
     */
    template <size_t BitsPerChar, size_t Base, size_t AlphabetSize, const char (&Alphabet) [AlphabetSize + 1], char PaddingChar = 0>
    class Impl {
        // minimum number of chars required for byte alignment ("quantum")
        static constexpr const size_t CharGroupSize = std::lcm(8, BitsPerChar) / BitsPerChar;

    public:
        /**
         * Encodes a given BufferRangeConst in and writes the result to BufferRange out, which is moved forward.
         * @param in Input to encode
         * @param out BufferRange that receives encoded data
         */
        static void encode(const BufferRangeConst &in, BufferRange &out) {
            constexpr auto t = createCodingTable<Base>(Alphabet, PaddingChar);
            const int mask = (1 << BitsPerChar) - 1;

            // encoded result string
            String result;
            // current bit value of input, carried bit value of last input byte
            uint8_t bitVal = 0, carryBitVal = 0;
            // input index, bits remaining in bitVal, size of carryBitVal
            uint32_t inIx = 0, bitsRem = 0, carryBitSize = 0;

            // while there are bytes in input, bits remaining in current input byte, or there is a carry value
            while (inIx < in.size() || bitsRem > 0 || carryBitSize > 0) {
                if (bitsRem >= BitsPerChar) {
                    // remaining input bits are enough for one output character
                    uint8_t b = (bitVal >> (bitsRem - BitsPerChar)) & mask;
                    size_t r = (carryBitVal << (BitsPerChar - carryBitSize)) + b;

                    result.append(&t.encoding[r], 1);
                    bitsRem -= BitsPerChar;
                    carryBitVal = carryBitSize = 0;
                } else {
                    // require more inputs bits for one output character

                    // create carry bit value and remember size
                    carryBitVal = bitVal & ((1 << bitsRem) - 1);
                    carryBitSize = bitsRem;

                    if (inIx >= in.size()) {
                        // no bits left in input, so pad with 0 bits
                        bitsRem += BitsPerChar - bitsRem;
                        bitVal = 0;
                    } else {
                        // pull in next byte from input
                        bitsRem += 8;
                        bitVal = *in.const_data(inIx++);
                    }
                }
            }

            // add padding characters so that result size is divisible by group size
            auto padCharCount = (CharGroupSize - (result.size() % CharGroupSize)) % CharGroupSize;
            if (t.paddingChar != 0)
                for (size_t i = 0; i < padCharCount; i++)
                    result.append(&t.paddingChar, 1);

            out.write(result.const_data(), result.size());
            out += result.size();
        }
        /**
         * Encodes a given BufferRangeConst in and writes the result to the underlying Buffer of BufferRange out.
         * @param in Input to encode
         * @param out Buffer that receives encoded data
         */
        static void encodeTo(const BufferRangeConst &in, BufferRange out) {
            encode(in, out);
        }
        /**
         * Encodes a given BufferRangeConst in and returns the encoded data.
         * @param in Input to encode
         * @return Encoded data
         */
        static String encode(const BufferRangeConst &in) {
            String result;
            encodeTo(in, result);
            return result;
        }
        /**
         * Encodes a given string and returns the encoded data.
         * @param in Input to encode
         * @return Encoded data
         */
        static std::string encodeString(const std::string &in) {
            return encode(String(in)).stl_str();
        }

        /**
         * Decodes a given BufferRangeConst in and writes the result to BufferRange out, which is moved forward. If
         * strict is enabled, decoding fails if it encounters an unknown character or invalid padding. In case of an
         * error, out is not modified.
         * @param in Input to decode
         * @param out BufferRange that receives decoded data
         * @param strict Strict decoding
         * @return Decoding result. If false is returned, out is not modified.
         */
        static bool decode(const BufferRangeConst &in, BufferRange &out, bool strict = false) {
            constexpr auto t = createCodingTable<Base>(Alphabet, PaddingChar);

            // ensure input is correctly padded if strict requested and a padding character is set
            if (strict && t.paddingChar != 0 && (in.size() % CharGroupSize != 0))
                return false;

            // decoded result string
            Buffer result;
            // current bit value of input, carried bit value of last input byte
            uint8_t bitVal, carryBitVal = 0;
            // size of carryBitVal, tracking number of padding characters
            int carryBitSize = 0, paddingCount = 0;
            // set to true if the first padding character is found
            bool foundPadding = false;

            for (size_t i = 0; i < in.size(); ++i) {
                // decoded bit value of character
                bitVal = t.decoding[*in.const_data(i)];

                if (*in.const_data(i) == t.paddingChar) {
                    // if this is a padding character, take note of it
                    paddingCount++;
                    foundPadding = true;
                } else if (strict && foundPadding)
                    // do not allow non-padding characters after a padding character
                    return false;

                // if this is an invalid character, continue or abort depending on strictness
                if (bitVal == t.invalidChar) {
                    if (strict)
                        return false;
                    continue;
                }

                // if enough bits to decode character to single byte
                if (carryBitSize + BitsPerChar >= 8) {
                    // byte value: carried bit value OR bit value
                    uint8_t d = (carryBitVal << (8 - carryBitSize)) | (bitVal >> (BitsPerChar - (8 - carryBitSize)));

                    // only add decoded byte to result if it is no padding
                    if (paddingCount == 0)
                        result.append(&d, 1);
                    else
                        paddingCount--;

                    carryBitSize = BitsPerChar - (8 - carryBitSize);
                    carryBitVal = bitVal & ((1 << carryBitSize) - 1);
                } else {
                    // not enough bits -> store bit value as carry
                    carryBitSize += BitsPerChar;
                    carryBitVal = (carryBitVal << BitsPerChar) | bitVal;
                }
            }

            out.write(result.const_data(), result.size());
            out += result.size();

            return true;
        }

        /**
         * Decodes a given BufferRangeConst in and writes the result to the underlying Buffer of out. If strict is
         * enabled, decoding fails if it encounters an unknown character or invalid padding. In case of an error, out is
         * not modified.
         * @param in Input to decode
         * @param out Buffer that receives decoded data
         * @param strict Strict decoding
         * @return Decoding result. If false is returned, out is not modified.
         */
        static bool decodeFrom(const BufferRangeConst &in, BufferRange out, bool strict = false) {
            return decode(in, out, strict);
        }
        /**
         * Decodes a given String and returns the decoded data. If strict is enabled, decoding fails if it encounters
         * an unknown character or invalid padding. In case of an error, an empty Buffer is returned.
         * @param in Input to decode
         * @param strict Strict decoding
         * @return Decoded data or empty Buffer if decoding failed.
         */
        static Buffer decode(const String &in, bool strict = false) {
            Buffer result;
            decodeFrom(in, result, strict);
            return result;
        }
        /**
         * Decodes a given string and returns the decoded data. If strict is enabled, decoding fails if it encounters
         * an unknown character or invalid padding. In case of an error, an empty Buffer is returned.
         * @param in Input to decode
         * @param strict Strict decoding
         * @return Decoded data or empty string if decoding failed.
         */
        static std::string decodeString(const std::string &in, bool strict = false) {
            return String(decode(String(in), strict)).stl_str();
        }
    };

    inline static constexpr char AlphaHex[] = "0123456789ABCDEF0123456789abcdef";
    inline static constexpr char Alpha16[] = "0123456789ABCDEF";
    inline static constexpr char Alpha32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    inline static constexpr char Alpha64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    class Hex : public Impl<4, 16, 32, AlphaHex, '='> {};
    class Base16 : public Impl<4, 16, 16, Alpha16, '='> {};
    class Base32 : public Impl<5, 32, 32, Alpha32, '='> {};
    class Base64 : public Impl<6, 64, 64, Alpha64, '='> {};
};

using Hex = BaseN::Hex;
using Base16 = BaseN::Base16;
using Base32 = BaseN::Base32;
using Base64 = BaseN::Base64;

#endif //SECUREMEMORY_BASEN_H
