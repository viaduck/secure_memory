/*
 * Copyright (C) 2015-2018 The ViaDuck Project
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

#include "ConversionTest.h"

#include <secure_memory/conversions.h>

/*
 * Note:
 * big endian does not swap bytes, therefore needs special no-op tests
 */

TEST_F(ConversionTest, Hton) {
    #if __BYTE_ORDER == LITTLE_ENDIAN
        EXPECT_EQ(0x12u, hton(static_cast<uint8_t>(0x12)));
        EXPECT_EQ(0x3412u, hton(static_cast<uint16_t>(0x1234)));
        EXPECT_EQ(0x78563412u, hton(static_cast<uint32_t>(0x12345678)));
        EXPECT_EQ(0x5634129078563412u, hton(static_cast<uint64_t>(0x1234567890123456)));
    #else
        EXPECT_EQ(0x12u, hton(static_cast<uint8_t>(0x12)));
        EXPECT_EQ(0x1234u, hton(static_cast<uint16_t>(0x1234)));
        EXPECT_EQ(0x12345678u, hton(static_cast<uint32_t>(0x12345678)));
        EXPECT_EQ(0x1234567890123456u, hton(static_cast<uint64_t>(0x1234567890123456)));
    #endif
}

TEST_F(ConversionTest, HtonFloat) {
    #if __BYTE_ORDER == LITTLE_ENDIAN
        EXPECT_NE(1.23f, hton(1.23f));
        EXPECT_EQ(1.23f, hton(hton(1.23f)));
    #else
        EXPECT_EQ(1.23f, hton(1.23f));
        EXPECT_EQ(1.23f, hton(hton(1.23f)));
    #endif
}

TEST_F(ConversionTest, HtonDouble) {
    #if __BYTE_ORDER == LITTLE_ENDIAN
        EXPECT_NE(3.14159, hton(3.14159));
        EXPECT_EQ(3.14159, hton(hton(3.14159)));
    #else
        EXPECT_EQ(3.14159, hton(3.14159));
        EXPECT_EQ(3.14159, hton(hton(3.14159)));
    #endif
}

TEST_F(ConversionTest, HtonComplex) {
    struct ComplexStruct {
        float a;
        int b;
        double c;

        bool operator==(const ComplexStruct &other) const {
            return a == other.a && b == other.b && c == other.c;
        }

        bool operator!=(const ComplexStruct &other) const {
            return !operator==(other);
        }
    };
    float finf = std::numeric_limits<float>::infinity();
    double dinf = std::numeric_limits<double>::infinity();

    ComplexStruct a = {finf, 0, dinf};
    ComplexStruct b = {-finf, 5, -dinf};
    ComplexStruct c = {1.234f, 5, 3.14159};

    #if __BYTE_ORDER == LITTLE_ENDIAN
        EXPECT_NE(a, hton(a));
        EXPECT_EQ(a, hton(hton(a)));
        EXPECT_NE(b, hton(b));
        EXPECT_EQ(b, hton(hton(b)));
        EXPECT_NE(c, hton(c));
        EXPECT_EQ(c, hton(hton(c)));
    #else
        EXPECT_EQ(a, hton(a));
        EXPECT_EQ(a, hton(hton(a)));
        EXPECT_EQ(b, hton(b));
        EXPECT_EQ(b, hton(hton(b)));
        EXPECT_EQ(c, hton(c));
        EXPECT_EQ(c, hton(hton(c)));
    #endif
}