/*
 * This file is part of the xylonx distribution (https://github.com/xylonx).
 * Copyright (c) 2022 xylonx.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "argparse.h"

#include <string>

#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

// Demonstrate some basic assertions.
TEST(OptParseStringParseTest, BasicAssertions) {
    char addr[16]      = {0};
    char address[4096] = {0};

    xserver::OptParse parser;
    parser.AddOption("addr", addr, 16, "0.0.0.0");
    parser.AddOption("address", address, 4096, "./");

    int argc           = 2;
    const char* data[] = {
        "--addr",
        "192.168.1.1",
    };
    parser.Parse(argc, reinterpret_cast<const char**>(data));

    EXPECT_EQ(std::string(addr), "192.168.1.1");
    EXPECT_EQ(std::string(address), "./");
}

TEST(OptParseIntParseTest, BasicAssertions) {
    int port;
    int pp;
    xserver::OptParse parser;
    parser.AddOption("port", &port);
    parser.AddOption("pp", &pp, 5555);
    int argc           = 2;
    const char* data[] = {
        "--port",
        "12",
    };
    parser.Parse(argc, reinterpret_cast<const char**>(data));

    EXPECT_EQ(port, 12);
    EXPECT_EQ(pp, 5555);
}

TEST(OptParseBoolParseTest, BasicAssertions) {
    bool b1 = false;
    bool b2 = true;

    xserver::OptParse parser;
    parser.AddOption("b", &b1);
    parser.AddOption("bb", &b2);
    int argc           = 2;
    const char* data[] = {
        "--b",
    };
    parser.Parse(argc, reinterpret_cast<const char**>(data));

    EXPECT_EQ(b1, true);
    EXPECT_EQ(b2, false);
}