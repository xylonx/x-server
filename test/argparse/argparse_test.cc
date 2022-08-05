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

#include "globals.h"
#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

// Demonstrate some basic assertions.
TEST(OptParseStringParseTest, BasicAssertions) {
    char addr[16]      = {0};
    char address[4096] = {0};

    xserver::OptParse parser;
    parser.AddOption("addr", addr, 16, "0.0.0.0");
    parser.AddOption("address", address, 4096, "./");

    int argc           = 12;
    const char* data[] = {
        "--addr",
        "192.168.1.1",
    };
    parser.Parse(argc, reinterpret_cast<const char**>(data));

    EXPECT_EQ(std::string(addr), "192.168.1.1");
    EXPECT_EQ(std::string(address), "./");
}
