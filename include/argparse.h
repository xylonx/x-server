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

#ifndef OPT_H
#define OPT_H

#include <any>
#include <map>
#include <utility>
#include <variant>
class OptParse {
public:
    OptParse();

    template <typename T>
    void add_option(const char* opt, const char* short_opt, T* target, T default_value);

    /**
     * @brief parse options defined by add_option method
     *
     * @param opt splitted option string
     */
    void parse(const char** opt);
};

#endif /* end OPT_H */