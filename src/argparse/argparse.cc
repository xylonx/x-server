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

#include <cstdlib>
#include <functional>

namespace xserver {

OptParse::OptParse() { root_ = new TrieNode(); };

OptParse::~OptParse() { DestroyTrieTree(root_); }

void OptParse::InsertToTrie(const char* key, const std::function<void(char*)>* assign) {
    InsertToTrie(key, assign, nullptr);
};

// TODO(xylonx): impl
void OptParse::InsertToTrie(const char* key, const std::function<void(char*)>* assign,
                            const std::function<void()>* set_defaults){

};

// TODO(xylonx): impl
void OptParse::FindLongestMatch(const char* argv) {}

void OptParse::DestroyTrieTree(struct TrieNode* root) {
    if (root != nullptr) {
        for (auto node : root->next_) {
            DestroyTrieTree(node);
        }
        delete root->set_;
        delete root->set_default_;
        delete root;
    }
}

void OptParse::AddOption(const char* opt, int* target) {
    if (opt != nullptr && target != nullptr) {
        auto assign = new std::function<void(char*)>([target](const char* value) { *target = atoi(value); });
        InsertToTrie(opt, assign);
    }
}

void OptParse::AddOption(const char* opt, int* target, int defaults) {
    if (opt != nullptr && target != nullptr) {
        auto assign       = new std::function<void(char*)>([target](const char* value) { *target = atoi(value); });
        auto set_defaults = new std::function<void()>([target, defaults]() { *target = defaults; });
        InsertToTrie(opt, assign, set_defaults);
    }
};

// TODO(xylonx): impl
void OptParse::AddOption(const char* opt, bool* target, bool defaults) {}

// TODO(xylonx): impl
void OptParse::AddOption(const char* opt, char* target, size_t len) {}

// TODO(xylonx): impl
void OptParse::AddOption(const char* opt, char* target, size_t len, const char* defaults) {}

// TODO(xylonx): impl
void OptParse::Parse(int argc, const char** argv) {}

}  // namespace xserver