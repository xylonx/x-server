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

#include <string>

#include "argparse.h"
#include "except.h"
#include "globals.h"
#include "http/server.h"
#include "spdlog/spdlog.h"

int main(int argc, char const* argv[]) {
    char addr[16]   = {0};
    char root[4096] = {0};
    char index[16]  = {0};
    int port        = 0;

    xserver::OptParse parser;
    parser.AddOption("addr", addr, 16, "0.0.0.0");
    parser.AddOption("port", &port, 7788);
    parser.AddOption("root", root, 4096, ".");
    parser.AddOption("index", index, 16, "");
    parser.Parse(argc, argv);

    xserver::HTTPServer server;
    try {
        server.Serve(addr, port, root, index);
    } catch (const xserver::XServerExcept& e) {
        spdlog::error("http server serve failed: {}", e.what());
    }
}