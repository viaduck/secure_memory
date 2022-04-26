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

#include <secure_memory/BaseN.h>
#include "BaseNTest.h"

class Base64NoPadding : public BaseN::Impl<6, 64, 64, BaseN::Alpha64> {};

// decode test: decode(BufferRange), decodeFrom, decode(String), decodeString
template<typename Coder>
static void BaseNTestDecode(bool strict, const char *in, const char *expectedOutput, bool expectedResult) {
    String strIn(in, std::strlen(in));
    BufferRangeConst rangeIn(strIn, 0, strIn.size());
    Buffer bufExpected(expectedOutput, std::strlen(expectedOutput));
    Buffer bufEmpty;

    // first check decoding methods without return value
    if (expectedResult) {
        // expected true, so decoded string must match
        EXPECT_EQ(Coder::decodeString(in, strict), expectedOutput) << "Wrong decoding output";
        EXPECT_EQ(Coder::decode(strIn, strict), bufExpected) << "Wrong decoding output";
    } else {
        // expected false, so decoded string must be bufEmpty
        EXPECT_EQ(Coder::decodeString(in, strict), "") << "Wrong decoding output";
        EXPECT_EQ(Coder::decode(strIn, strict), bufEmpty) << "Wrong decoding output";
    }

    // now check decoding methods with return value
    Buffer bufOut;
    BufferRange rangeOut = bufOut.end();
    EXPECT_EQ(expectedResult, Coder::decode(rangeIn, rangeOut, strict)) << "Wrong decoding result";
    if (expectedResult) {
        EXPECT_EQ(bufOut, bufExpected) << "Wrong decoding output";
        // check that BufferRange has been modified
        EXPECT_EQ(rangeOut.offset(), bufExpected.size()) << "Wrong decoding output range";
    } else {
        EXPECT_EQ(bufOut, bufEmpty) << "Wrong decoding output";
        EXPECT_EQ(rangeOut.offset(), 0u) << "Wrong decoding output range";
    }

    bufOut.clear();

    BufferRange rangeOut2 = bufOut.end();
    EXPECT_EQ(expectedResult, Coder::decodeFrom(rangeIn, rangeOut2, strict)) << "Wrong decoding result";
    if (expectedResult) {
        EXPECT_EQ(bufOut, bufExpected) << "Wrong decoding output";
        // check that BufferRange has not been modified
        EXPECT_EQ(rangeOut2.offset(), 0u) << "Wrong decoding output range";
    } else {
        EXPECT_EQ(bufOut, bufEmpty) << "Wrong decoding output";
        EXPECT_EQ(rangeOut2.offset(), 0u) << "Wrong decoding output range";
    }
}

// encode test: encode(BufferRange, BufferRange out) -> void, encodeTo, encode(BufferRange) -> String, encodeString
template<typename Coder>
static void BaseNTestEncode(const char *in, const char *expectedOutput) {
    String strIn(in, std::strlen(in));
    BufferRangeConst rangeIn(strIn, 0, strIn.size());

    Buffer bufExpected(expectedOutput, std::strlen(expectedOutput));
    String strExpected(expectedOutput);

    Buffer bufEmpty;

    EXPECT_EQ(Coder::encodeString(in), expectedOutput) << "Wrong encoding output";
    EXPECT_EQ(Coder::encode(rangeIn), strExpected) << "Wrong encoding output";

    // now check decoding methods with return value
    Buffer bufOut;
    BufferRange rangeOut = bufOut.end();
    Coder::encode(rangeIn, rangeOut);
    EXPECT_EQ(bufOut, bufExpected) << "Wrong decoding output";
    // check that BufferRange has been modified
    EXPECT_EQ(rangeOut.offset(), bufExpected.size()) << "Wrong decoding output range";

    bufOut.clear();

    BufferRange rangeOut2 = bufOut.end();
    Coder::encodeTo(rangeIn, rangeOut2);
    EXPECT_EQ(bufOut, bufExpected) << "Wrong decoding output";
    // check that BufferRange has not been modified
    EXPECT_EQ(rangeOut2.offset(), 0u) << "Wrong decoding output range";
}

template<typename Coder>
static void BaseNTestBoth(bool strict, const char *decoded, const char *encoded, bool expectedResult) {
    BaseNTestDecode<Coder>(strict, encoded, decoded, expectedResult);
    BaseNTestEncode<Coder>(decoded, encoded);
}

TEST_F(BaseNTest, testBase64) {
    BaseNTestBoth<Base64>(true, "", "", true);
    BaseNTestBoth<Base64>(true, "Man", "TWFu", true);
    BaseNTestBoth<Base64>(true, "Ma", "TWE=", true);
    BaseNTestBoth<Base64>(true, "M", "TQ==", true);
    BaseNTestBoth<Base64>(true, "123", "MTIz", true);
    BaseNTestBoth<Base64>(true, "f", "Zg==", true);
    BaseNTestBoth<Base64>(true, "fo", "Zm8=", true);
    BaseNTestBoth<Base64>(true, "foo", "Zm9v", true);
    BaseNTestBoth<Base64>(true, "foob", "Zm9vYg==", true);
    BaseNTestBoth<Base64>(true, "fooba", "Zm9vYmE=", true);
    BaseNTestBoth<Base64>(true, "foobar", "Zm9vYmFy", true);
    BaseNTestDecode<Base64>(true, "Zg==TQ==", "", false);
    BaseNTestDecode<Base64>(false, "Zg==TQ==", "fM", true);
    BaseNTestDecode<Base64>(false, "Zg##==TQ==", "fM", true);
}

TEST_F(BaseNTest, testBase64NoPadding) {
    BaseNTestBoth<Base64NoPadding>(true, "", "", true);
    BaseNTestBoth<Base64NoPadding>(true, "Man", "TWFu", true);
    BaseNTestBoth<Base64NoPadding>(true, "Ma", "TWE", true);
    BaseNTestBoth<Base64NoPadding>(true, "M", "TQ", true);
    BaseNTestBoth<Base64NoPadding>(true, "123", "MTIz", true);
    BaseNTestBoth<Base64NoPadding>(true, "f", "Zg", true);
    BaseNTestBoth<Base64NoPadding>(true, "fo", "Zm8", true);
    BaseNTestBoth<Base64NoPadding>(true, "foo", "Zm9v", true);
    BaseNTestBoth<Base64NoPadding>(true, "foob", "Zm9vYg", true);
    BaseNTestBoth<Base64NoPadding>(true, "fooba", "Zm9vYmE", true);
    BaseNTestBoth<Base64NoPadding>(true, "foobar", "Zm9vYmFy", true);
    BaseNTestDecode<Base64NoPadding>(true, "Zg==TQ==", "fM", false);
}

TEST_F(BaseNTest, testBase16) {
    BaseNTestBoth<Base16>(true, "abc", "616263", true);
    BaseNTestBoth<Base16>(true, "", "", true);

    // test invalid characters
    BaseNTestDecode<Base16>(false, "61zzZZ62", "ab", true);
    BaseNTestDecode<Base16>(true, "61zzZZ62", "", false);

    // test padding
    BaseNTestDecode<Base16>(true, "0", "", false);
    BaseNTestDecode<Base16>(true, "0=", "", true);
}

TEST_F(BaseNTest, testHex) {
    BaseNTestDecode<Hex>(false, "4a4b4c4A4B4C", "JKLJKL", true);
    BaseNTestEncode<Hex>("JKLJKL", "4A4B4C4A4B4C");

    BaseNTestBoth<Hex>(false, "JKLJKL", "4A4B4C4A4B4C", true);
}

TEST_F(BaseNTest, testBase32) {
    BaseNTestBoth<Base32>(true, "", "", true);
    BaseNTestBoth<Base32>(true, "f", "MY======", true);
    BaseNTestBoth<Base32>(true, "fo", "MZXQ====", true);
    BaseNTestBoth<Base32>(true, "foo", "MZXW6===", true);
    BaseNTestBoth<Base32>(true, "foob", "MZXW6YQ=", true);
    BaseNTestBoth<Base32>(true, "fooba", "MZXW6YTB", true);
    BaseNTestBoth<Base32>(true, "foobar", "MZXW6YTBOI======", true);
}
