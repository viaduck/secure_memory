/*
 * Copyright (C) 2020 The ViaDuck Project
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

#include <cstdint>
#include <limits>

#include <secure_memory/SafeInt.h>

#include "SafeIntTest.h"

TEST_F(SafeIntTest, Create) {
    SafeInt<uint32_t> siMax(std::numeric_limits<uint32_t>::max());
    SafeInt<uint32_t> siMin(std::numeric_limits<uint32_t>::lowest());
    SafeInt<int32_t> si2Min(std::numeric_limits<int32_t>::lowest());
    SafeInt<int16_t> si3Min(std::numeric_limits<int16_t>::lowest());
    SafeInt<int32_t> si4Min(std::numeric_limits<int16_t>::lowest());
    EXPECT_EQ(std::numeric_limits<uint32_t>::max(), static_cast<uint32_t>(siMax));
    EXPECT_EQ(std::numeric_limits<uint32_t>::lowest(), static_cast<uint32_t>(siMin));
    EXPECT_EQ(std::numeric_limits<int32_t>::lowest(), static_cast<int32_t>(si2Min));
    EXPECT_EQ(std::numeric_limits<int16_t>::lowest(), static_cast<int16_t>(si3Min));
    EXPECT_EQ(std::numeric_limits<int16_t>::lowest(), static_cast<int16_t>(si4Min));
}

TEST_F(SafeIntTest, Add) {
    uint16_t uint16_min = std::numeric_limits<uint16_t>::lowest();
    uint16_t uint16_max = std::numeric_limits<uint16_t>::max();
    int16_t int16_min = std::numeric_limits<int16_t>::lowest();
    int16_t int16_max = std::numeric_limits<int16_t>::max();

    // basic arithmetic
    EXPECT_EQ(0, 0_si + 0_si);
    EXPECT_EQ(uint16_min, make_si(uint16_min) + 0_si16);
    EXPECT_EQ(uint16_min, 0_si16 + make_si(uint16_min));
    EXPECT_EQ(uint16_max, make_si(uint16_max) + 0_si16);
    EXPECT_EQ(uint16_max, 0_si16 + make_si(uint16_max));

    EXPECT_EQ(int16_min, make_si(int16_min) + 0_ssi16);
    EXPECT_EQ(int16_min, 0_ssi16 + make_si(int16_min));
    EXPECT_EQ(int16_max, make_si(int16_max) + 0_ssi16);
    EXPECT_EQ(int16_max, 0_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max, -0_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max, make_si(int16_max) + -0_ssi16);
    EXPECT_EQ(int16_max - 1, make_si(int16_max) + -1_ssi16);
    EXPECT_EQ(int16_max - 1, -1_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max - 1, make_si(int16_max) + 0_ssi16 + -1_ssi16);
    EXPECT_EQ(int16_max - 1, make_si(int16_max) + -1_ssi16 + 0_ssi16);
    EXPECT_EQ(int16_max - 1, make_si(int16_max) + -1_ssi16 + -0_ssi16);
    EXPECT_EQ(int16_max - 1, make_si(int16_max) + -0_ssi16 + -1_ssi16);
    EXPECT_EQ(int16_max - 1, 0_ssi16 + -1_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max - 1, -0_ssi16 + -1_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max - 1, -1_ssi16 + 0_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max - 1, -1_ssi16 + -0_ssi16 + make_si(int16_max));

    // check overflow
    EXPECT_EQ(uint16_max, make_si(uint16_max) + 1_si16);
    EXPECT_EQ(uint16_max, 1_si16 + make_si(uint16_max));
    EXPECT_EQ(uint16_max, make_si(uint16_max) + 0_si16 + 1_si16);
    EXPECT_EQ(uint16_max, make_si(uint16_max) + 1_si16 + 0_si16);
    EXPECT_EQ(uint16_max, 0_si16 + 1_si16 + make_si(uint16_max));
    EXPECT_EQ(uint16_max, 1_si16 + 0_si16 + make_si(uint16_max));

    EXPECT_EQ(int16_max, make_si(int16_max) + 1_ssi16);
    EXPECT_EQ(int16_max, 1_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max, make_si(int16_max) + 0_ssi16 + 1_ssi16);
    EXPECT_EQ(int16_max, make_si(int16_max) + 1_ssi16 + 0_ssi16);
    EXPECT_EQ(int16_max, 0_ssi16 + 1_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_max, 1_ssi16 + 0_ssi16 + make_si(int16_max));
    EXPECT_EQ(int16_min, make_si(int16_min) + -1_ssi16);
    EXPECT_EQ(int16_min, -1_ssi16 + make_si(int16_min));
    EXPECT_EQ(int16_min, make_si(int16_min) + 0_ssi16 + -1_ssi16);
    EXPECT_EQ(int16_min, make_si(int16_min) + -1_ssi16 + 0_ssi16);
    EXPECT_EQ(int16_min, 0_ssi16 + -1_ssi16 + make_si(int16_min));
    EXPECT_EQ(int16_min, -1_ssi16 + 0_ssi16 + make_si(int16_min));
}

TEST_F(SafeIntTest, Sub) {
    uint16_t uint16_min = std::numeric_limits<uint16_t>::lowest();
    uint16_t uint16_max = std::numeric_limits<uint16_t>::max();
    int16_t int16_min = std::numeric_limits<int16_t>::lowest();
    int16_t int16_max = std::numeric_limits<int16_t>::max();

    // basic arithmetic
    EXPECT_EQ(0, 0_si - 0_si);
    EXPECT_EQ(uint16_min, make_si(uint16_min) - 0_si16);
    EXPECT_EQ(uint16_min, 0_si16 - make_si(uint16_min));
    EXPECT_EQ(uint16_max, make_si(uint16_max) - 0_si16);
    EXPECT_EQ(uint16_min, 0_si16 - make_si(uint16_max));

    EXPECT_EQ(int16_min, make_si(int16_min) - 0_ssi16);
    EXPECT_EQ(int16_max, 0_ssi16 - make_si(int16_min));
    EXPECT_EQ(int16_max, make_si(int16_max) - 0_ssi16);
    EXPECT_EQ(-int16_max, 0_ssi16 - make_si(int16_max));
    EXPECT_EQ(-int16_max, -0_ssi16 - make_si(int16_max));

    EXPECT_EQ(int16_max - 1, make_si(int16_max) - 1_ssi16);
    EXPECT_EQ(int16_min, -1_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_max, make_si(int16_max) - 0_ssi16 - -1_ssi16);
    EXPECT_EQ(int16_max, make_si(int16_max) - -1_ssi16 - 0_ssi16);
    EXPECT_EQ(int16_max, make_si(int16_max) - -1_ssi16 - -0_ssi16);
    EXPECT_EQ(int16_max, make_si(int16_max) - -0_ssi16 - -1_ssi16);
    EXPECT_EQ(int16_min + 2, 0_ssi16 - -1_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_min + 2, -0_ssi16 - -1_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_min, -1_ssi16 - 0_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_min, -1_ssi16 - -0_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_max, make_si(int16_max) - -1_ssi16);

    // check overflow
    EXPECT_EQ(uint16_min, make_si(uint16_min) - 1_si16);
    EXPECT_EQ(uint16_min, 1_si16 - make_si(uint16_max));
    EXPECT_EQ(uint16_min, make_si(uint16_min) - 0_si16 - 1_si16);
    EXPECT_EQ(uint16_min, make_si(uint16_min) - 1_si16 - 0_si16);
    EXPECT_EQ(uint16_min, 0_si16 - 1_si16 - make_si(uint16_max));
    EXPECT_EQ(uint16_min, 1_si16 - 0_si16 - make_si(uint16_max));

    EXPECT_EQ(int16_min, make_si(int16_min) - 1_ssi16);
    EXPECT_EQ(int16_min, -2_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_min, make_si(int16_min) - 0_ssi16 - 1_ssi16);
    EXPECT_EQ(int16_min, make_si(int16_min) - 1_ssi16 - 0_ssi16);
    EXPECT_EQ(int16_min, 0_ssi16 - 2_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_min, -2_ssi16 - 0_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_max, make_si(int16_max) - -1_ssi16);
    EXPECT_EQ(int16_max, -1_ssi16 - make_si(int16_min));
    EXPECT_EQ(int16_max, make_si(int16_max) - 0_ssi16 - -1_ssi16);
    EXPECT_EQ(int16_max, make_si(int16_max) - -1_ssi16 - 0_ssi16);
    EXPECT_EQ(int16_min, 0_ssi16 + -1_ssi16 - make_si(int16_max));
    EXPECT_EQ(int16_min, -1_ssi16 - 0_ssi16 - make_si(int16_max));
}

TEST_F(SafeIntTest, Mul) {
    uint16_t uint16_min = std::numeric_limits<uint16_t>::lowest();
    uint16_t uint16_max = std::numeric_limits<uint16_t>::max();
    int16_t int16_min = std::numeric_limits<int16_t>::lowest();
    int16_t int16_max = std::numeric_limits<int16_t>::max();

    // basic arithmetic
    EXPECT_EQ(0, 0_si * 0_si);
    EXPECT_EQ(1, 1_si * 1_si);
    EXPECT_EQ(1, -1_ssi * -1_ssi);
    EXPECT_EQ(-1, -1_ssi * 1_ssi);
    EXPECT_EQ(-1, 1_ssi * -1_ssi);
    EXPECT_EQ(uint16_min, make_si(uint16_min) * 1_si16);
    EXPECT_EQ(uint16_min, 1_si16 * make_si(uint16_min));
    EXPECT_EQ(uint16_max, make_si(uint16_max) * 1_si16);
    EXPECT_EQ(uint16_max, 1_si16 * make_si(uint16_max));
    EXPECT_EQ(int16_min, make_si(int16_min) * 1_ssi16);
    EXPECT_EQ(int16_min, 1_ssi16 * make_si(int16_min));
    EXPECT_EQ(int16_max, make_si(int16_max) * 1_ssi16);
    EXPECT_EQ(int16_max, 1_ssi16 * make_si(int16_max));

    // check overflow
    EXPECT_EQ(uint16_max, make_si(uint16_max) * 2_si16);
    EXPECT_EQ(uint16_max, 2_si16 * make_si(uint16_max));
    EXPECT_EQ(int16_min, make_si(int16_min) * 2_ssi16);
    EXPECT_EQ(int16_min, 2_ssi16 * make_si(int16_min));
    EXPECT_EQ(int16_max, make_si(int16_min) * -1_ssi16);
    EXPECT_EQ(int16_max, -1_ssi16 * make_si(int16_min));
}
