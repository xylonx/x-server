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

#include <cstddef>
#include <functional>

namespace xserver {

// match globals.h addr

class OptParse {
public:
    OptParse();

    ~OptParse();

    /**
     * @brief add option with option value
     *
     * @param opt opt str without leading - or --
     * @param target where to store parsed value. if target is nullptr, the result will be ignored
     */
    void AddOption(const char* opt, int* target);

    /**
     * @brief add option with option value. if opt is not found, it will be assigned as defaults value
     *
     * @param opt opt str without leading - or --
     * @param target where to store parsed value. if target is nullptr, the result will be ignored
     */
    void AddOption(const char* opt, int* target, int defaults);

    /**
     * @brief same as AddOption(const char* opt, int* target) but store bool value. And it is flag only, which means
     * that when meets `--bool_opt`, parser will set target as !defaults
     */
    void AddOption(const char* opt, bool* target, bool defaults = false);

    /**
     * @brief same as AddOption(const char* opt, int* target) but store string value. It will store detected value into
     * target with max len `len`
     */
    void AddOption(const char* opt, char* target, size_t len);

    /**
     * @brief same as AddOption(const char* opt, char* target, size_t len) but with defaults value
     */
    void AddOption(const char* opt, char* target, size_t len, const char* defaults);

    /**
     * @brief parse options defined by add_option method
     *
     * @param opt splitted option string
     */
    void Parse(int argc, const char** argv);

private:
    struct TrieNode {
        std::function<void(const char*)>* set_;  // value assign function
        bool is_flag_;
        struct TrieNode* next_[27];  // magic number means 26+1 (a-z_)

        TrieNode() = default;
    };

    void InsertToTrie(const char* key, std::function<void(const char*)>* assign, bool is_flag = false);
    TrieNode* FindLongestMatch(const char* argv);
    void DestroyTrieTree(TrieNode* root);

    TrieNode* root_;
};

}  // namespace xserver

#endif /* end OPT_H */