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

#include "StringTest.h"
#include "secure_memory/String.h"
#include "custom_assert.h"


TEST(StringTest, creationTest) {
    // nullptr
    {
        String s(static_cast<const char*>(nullptr));
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
    }
    {
        String s(static_cast<const uint8_t*>(nullptr), 10);
        ASSERT_EQ(10, static_cast<int32_t>(s.size()));              // according to changed API: Buffer::write(..) increases and uses memory if nullptr supplied
        String s2(static_cast<const uint8_t*>(nullptr), 0);
        ASSERT_EQ(0, static_cast<int32_t>(s2.size()));
    }
    // default constructor
    {
        String s;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()));
    }

    // create from cstring
    {
        String s("");
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()));
    }
    {
        String s("abc");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size())+1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "abc", s.const_data(), static_cast<int32_t>(s.size()));
    }

    // create from byte sequence
    ASSERT_EQ(1, static_cast<int32_t>(sizeof(char)));
    {
        uint8_t *bytes = nullptr;
        String s(bytes, 0);
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()));
    }
    {
        uint8_t bytes[] = {1,2,3,4,5,6, 255, 127, 0};
        String s(bytes, 9);
        ASSERT_EQ(9, static_cast<int32_t>(s.size()));

        uint8_t res[] = {1,2,3,4,5,6, 255, 127, 0, 0};
        EXPECT_ARRAY_EQ(const char, reinterpret_cast<const char *>(res), s.c_str(), static_cast<int32_t>(s.size())+1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, reinterpret_cast<const char *>(res), s.const_data(), static_cast<int32_t>(s.size()));
    }

    // create from String
    {
        String str("");
        String s(str);
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()));
    }
    {
        String str("abc");
        String s(str);
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "abc", s.const_data(), static_cast<int32_t>(s.size()));
    }

    // create from Buffer
    {
        Buffer b;
        String s(b);
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()));
    }
    {
        String b("abcdefghijklmnopq", 17);
        String s(b);
        ASSERT_EQ(17, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abcdefghijklmnopq", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "abcdefghijklmnopq", s.const_data(), static_cast<int32_t>(s.size()));
    }

    // create from stl string
    {
        std::string stl;
        String s(stl);
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()));
    }
    {
        std::string stl("abc");
        String s(stl);
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
        EXPECT_ARRAY_EQ(const char, "abc", s.const_data(), static_cast<int32_t>(s.size()));
    }
}

TEST(StringTest, appendTest) {
    // nullptr
    {
        String s("abc");
        String s2(nullptr);
        String sf = s + s2;
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s(nullptr);
        String s2("abc");
        String sf = s + s2;
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        String s2(nullptr);
        s += s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s(nullptr);
        String s2("abc");
        s += s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    // ## append a String
    {
        String s("abc");
        String s2("");
        String sf = s + s2;
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        String s2("def");
        String sf = s + s2;
        ASSERT_EQ(6, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        String s2("abc");
        String sf = s + s2;
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    // +=
    {
        String s("abc");
        String s2("");
        s += s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        String s2("def");
        s += s2;
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        String s2("abc");
        s += s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }

    // ## append a cstring
    {
        String s("");
        String sf = s + "abc";
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        String sf = s + "def";
        ASSERT_EQ(6, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        String sf = s + "";
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    // +=
    {
        String s("");
        s += "abc";
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        s += "def";
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        s += "";
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }

    // ## append an stl string
    {
        String s("abc");
        std::string s2;
        String sf = s + s2;
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        std::string s2("def");
        String sf = s + s2;
        ASSERT_EQ(6, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        std::string s2("abc");
        String sf = s + s2;
        ASSERT_EQ(3, static_cast<int32_t>(sf.size()));
        EXPECT_ARRAY_EQ(const char, "abc", sf.c_str(), static_cast<int32_t>(sf.size()) + 1);       // compare the 0-terminator, too!
    }
    // +=
    {
        String s("abc");
        std::string s2;
        s += s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("abc");
        std::string s2("def");
        s += s2;
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        std::string s2("abc");
        s += s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
}

TEST(StringTest, compareTest) {
    // nullptr
    {
        String s("");
        String s2(nullptr);
        ASSERT_TRUE(s == s2);
        ASSERT_FALSE(s != s2);
    }
    {
        String s(nullptr);
        String s2("");
        ASSERT_TRUE(s == s2);
        ASSERT_FALSE(s != s2);
    }
    {
        String s("");
        ASSERT_FALSE(s == nullptr);
        ASSERT_TRUE(s != nullptr);
    }
    //
    {
        String s("");
        String s2("");
        ASSERT_TRUE(s == s2);
        ASSERT_FALSE(s != s2);
    }
    {
        String s("abc");
        String s2("abc");
        ASSERT_TRUE(s == s2);
        ASSERT_FALSE(s != s2);
    }
    {
        String s("");
        String s2("abc");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
    {
        String s("abc");
        String s2("");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
    {
        String s("abcd");
        String s2("abc");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
    {
        String s("abc");
        String s2("abcd");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }

    // cstrings
    {
        String s("");
        ASSERT_TRUE(s == "");
        ASSERT_FALSE(s != "");
    }
    {
        String s("abc");
        ASSERT_TRUE(s == "abc");
        ASSERT_FALSE(s != "abc");
    }
    {
        String s("");
        ASSERT_FALSE(s == "abc");
        ASSERT_TRUE(s != "abc");
    }
    {
        String s("abc");
        ASSERT_FALSE(s == "");
        ASSERT_TRUE(s != "");
    }
    {
        String s("abcd");
        ASSERT_FALSE(s == "abc");
        ASSERT_TRUE(s != "abc");
    }
    {
        String s("abc");
        String s2("abcd");
        ASSERT_FALSE(s == "abcd");
        ASSERT_TRUE(s != "abcd");
    }

    // stl strings
    {
        String s("");
        std::string s2("");
        ASSERT_TRUE(s == s2);
        ASSERT_FALSE(s != s2);
    }
    {
        String s("abc");
        std::string s2("abc");
        ASSERT_TRUE(s == s2);
        ASSERT_FALSE(s != s2);
    }
    {
        String s("");
        std::string s2("abc");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
    {
        String s("abc");
        std::string s2("");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
    {
        String s("abcd");
        std::string s2("abc");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
    {
        String s("abc");
        std::string s2("abcd");
        ASSERT_FALSE(s == s2);
        ASSERT_TRUE(s != s2);
    }
}

TEST(StringTest, emptyTest) {
    {
        String s("");
        ASSERT_TRUE(s.empty());
    }
    {
        String s(nullptr);
        ASSERT_TRUE(s.empty());
    }
    {
        String s(String(""));
        ASSERT_TRUE(s.empty());
    }
    {
        String s(std::string(""));
        ASSERT_TRUE(s.empty());
    }

    {
        String s("abc");
        ASSERT_FALSE(s.empty());
    }
    {
        String s(std::string("abc"));
        ASSERT_FALSE(s.empty());
    }
    {
        String s(String("abc"));
        ASSERT_FALSE(s.empty());
    }
}

TEST(StringTest, reassignTest) {
    // nullptr
    {
        String s("abc");
        String s2(nullptr);
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        ASSERT_EQ(0, static_cast<int32_t>(s2.size()));
        s = s2;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s(nullptr);
        String s2("abc");
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        ASSERT_EQ(3, static_cast<int32_t>(s2.size()));
        s = s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abc", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    // String
    {
        String s("abc");
        String s2("def");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        s = s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "def", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        String s2("def");
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        s = s2;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "def", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }

    // cstring
    {
        String s("abc");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        s = "def";
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "def", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        s = "def";
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "def", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }

    // stl string
    {
        String s("abc");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        s = std::string("def");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "def", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
    {
        String s("");
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        s = std::string("def");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "def", s.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
}

TEST(StringTest, conversionTest) {
    // cstring
    {
        String s("abc");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        const char *cs1 = s.c_str();
        EXPECT_ARRAY_EQ(const char, "abc", cs1, static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!

        s = s + "def";
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        const char *cs2 = s.c_str();
        EXPECT_ARRAY_EQ(const char, "abcdef", cs2, static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!

        // old pointer must retain its value!
        EXPECT_ARRAY_EQ(const char, "abc", cs1, 3 + 1);       // compare the 0-terminator, too!
    }

    // stl
    {
        String s("abc");
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        std::string std1 = s.stl_str();
        EXPECT_ARRAY_EQ(const char, "abc", std1.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!

        s = s + "def";
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        std::string std2 = s.stl_str();
        EXPECT_ARRAY_EQ(const char, "abcdef", std2.c_str(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!

        // old std::string must retain its value!
        EXPECT_ARRAY_EQ(const char, "abc", std1.c_str(), 3 + 1);       // compare the 0-terminator, too!
    }
}

TEST(StringTest, castTest) {
    String s("abc");
    ASSERT_EQ(3, static_cast<int32_t>(s.size()));
    EXPECT_ARRAY_EQ(const char, "abc", s.const_data(), static_cast<int32_t>(s.size()));      // String does not use any 0-terminator internally
}


// required for streaming test
struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
        this->setp(begin, end);
    }
};

TEST(StringTest, istreamTest) {
    {
        String s;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));

        char buffer[] = "abcdefg\0h1234i\nxyz9876";
        membuf sbuf(buffer, buffer + sizeof(buffer));
        std::istream in(&sbuf);

        in >> s;        // stream it!

        ASSERT_EQ(14, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abcdefg\0h1234i\0", s.const_data(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }

    {
        String s;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));

        char buffer[] = "";
        membuf sbuf(buffer, buffer);
        std::istream in(&sbuf);

        in >> s;        // stream it!

        ASSERT_EQ(0u, static_cast<int32_t>(s.size()));
    }

    {
        String s;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));

        char buffer[] = "abcdef";
        membuf sbuf(buffer, buffer + sizeof(buffer));
        std::istream in(&sbuf);

        in >> s;        // stream it!

        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "abcdef", s.const_data(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }

    {
        String s;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));

        char buffer[] = "";
        membuf sbuf(buffer, buffer + sizeof(buffer));
        std::istream in(&sbuf);

        in >> s;        // stream it!

        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.const_data(), static_cast<int32_t>(s.size()) + 1);       // compare the 0-terminator, too!
    }
}

TEST(StringTest, ostreamTest) {
    {
        String s("abcdefg\0h1234i\nxyz9876", 22);
        ASSERT_EQ(22, static_cast<int32_t>(s.size()));

        char buffer[512];
        memset(buffer, 0xAD, 512);
        membuf sbuf(buffer, buffer + sizeof(buffer));
        std::ostream out(&sbuf);
        sbuf.pubsync();
        out << s;        // stream it!

        EXPECT_ARRAY_EQ(const char, "abcdefg\0h1234i\nxyz9876", buffer, static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        String s("abcdef");
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));

        char buffer[512];
        memset(buffer, 0xAD, 512);
        membuf sbuf(buffer, buffer + sizeof(buffer));
        std::ostream out(&sbuf);
        sbuf.pubsync();

        out << s;        // stream it!

        EXPECT_ARRAY_EQ(const char, "abcdef", buffer, static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        String s("");
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));

        char buffer[512];
        memset(buffer, 0xAD, 512);
        membuf sbuf(buffer, buffer + sizeof(buffer));
        std::ostream out(&sbuf);
        sbuf.pubsync();

        out << s;        // stream it!

        EXPECT_ARRAY_EQ(const char, "", buffer, static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
}

TEST(StringTest, toHex) {
    // String::toHex(void*, int)
    {
        String s = String::toHex(nullptr, 10);
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        uint8_t bytes[] = {255, 1, 14, 3, 12, 5, 6, 255, 127, 189, 0};
        String s = String::toHex(bytes, 10);
        ASSERT_EQ(10 * 2, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "ff010e030c0506ff7fbd00", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        uint8_t bytes[] = {0};
        String s = String::toHex(bytes, 1);
        ASSERT_EQ(1 * 2, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "00", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        uint8_t *bytes = nullptr;
        String s = String::toHex(bytes, 0);
        ASSERT_EQ(0 * 2, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    // String::toHex()
    {
        String o;
        String s = o.toHex();
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        uint8_t bytes[] = {255, 1, 14, 3, 12, 5, 6, 255, 127, 189, 0};
        String o(bytes, 10);
        String s = o.toHex();
        ASSERT_EQ(10 * 2, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "ff010e030c0506ff7fbd00", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        uint8_t bytes[] = {0};
        String o(bytes, 1);
        String s = o.toHex();
        ASSERT_EQ(1 * 2, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "00", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
    {
        uint8_t *bytes = nullptr;
        String o(bytes, 0);
        String s = o.toHex();
        ASSERT_EQ(0 * 2, static_cast<int32_t>(s.size()));
        EXPECT_ARRAY_EQ(const char, "", s.const_data(),
                        static_cast<int32_t>(s.size()));       // String does not use any 0-terminator internally
    }
}

TEST(StringTest, toInt) {
    {
        String s("");
        uint32_t result;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        ASSERT_FALSE(s.toInt(10, result));
        ASSERT_EQ(0, static_cast<int32_t>(result));
    }
    {
        String s("");
        uint32_t result;
        ASSERT_EQ(0, static_cast<int32_t>(s.size()));
        ASSERT_FALSE(s.toInt(10, result));
        ASSERT_EQ(0, static_cast<int32_t>(result));
    }
    {
        String s("123");
        uint32_t result;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        ASSERT_TRUE(s.toInt(10, result));
        ASSERT_EQ(123, static_cast<int32_t>(result));
    }
    {
        String s("ab0");
        uint32_t result;
        ASSERT_EQ(3, static_cast<int32_t>(s.size()));
        ASSERT_TRUE(s.toInt(10, result));
        ASSERT_EQ(0, static_cast<int32_t>(result));
    }
    {
        String s("abcdef");
        uint32_t result;
        ASSERT_EQ(6, static_cast<int32_t>(s.size()));
        ASSERT_FALSE(s.toInt(10, result));
        ASSERT_EQ(0, static_cast<int32_t>(result));
    }
    {
        String s("ab0986bc0123def");
        uint32_t result;
        ASSERT_EQ(15, static_cast<int32_t>(s.size()));
        ASSERT_TRUE(s.toInt(10, result));
        ASSERT_EQ(9860123, static_cast<int32_t>(result));
    }
    {
        String s("ff");
        uint32_t result;
        ASSERT_EQ(2, static_cast<int32_t>(s.size()));
        ASSERT_TRUE(s.toInt(16, result));
        ASSERT_EQ(255, static_cast<int32_t>(result));
    }
    {
        String s("zx56efcdyw");
        uint32_t result;
        ASSERT_EQ(10, static_cast<int32_t>(s.size()));
        ASSERT_TRUE(s.toInt(16, result));
        ASSERT_EQ(0x56efcd, static_cast<int32_t>(result));
    }
}

TEST(StringTest, lessOperator) {
    {
        String s("test1"), p("test2");
        ASSERT_TRUE(s < p);
        ASSERT_FALSE(p < s);
    }
    {
        String s("aaa"), p("bb");
        ASSERT_TRUE(s < p);
        ASSERT_FALSE(p < s);
    }
    {
        String s("aa"), p("z");
        ASSERT_TRUE(s < p);
        ASSERT_FALSE(p < s);
    }
    {
        String q("test");
        ASSERT_FALSE(q < q);
    }
    {
        String q, p("test");
        ASSERT_FALSE(q < q);
        ASSERT_TRUE(q < p);
        ASSERT_FALSE(p < q);
    }
}

TEST(StringTest, hashOp) {
    {
        String test("test1"), testDiff("lkajsasjs"), testSim("test2");
        std::hash<const String> hasher;
        size_t hashTest = hasher(test);
        size_t hashDiff = hasher(testDiff);
        size_t hashSim = hasher(testSim);
        ASSERT_NE(hashTest, hashDiff);
        ASSERT_NE(hashTest, hashSim);
    }
}
