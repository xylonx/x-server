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
#include <cstring>
#include <functional>

#include "spdlog/spdlog.h"

namespace xserver {

OptParse::OptParse() { root_ = new TrieNode(); };

OptParse::~OptParse() { DestroyTrieTree(root_); }

void OptParse::InsertToTrie(const char* key, std::function<void(const char*)>* assign, bool is_flag) {
    auto node = root_;
    for (; key != nullptr && *key != '\0'; ++key) {
        int idx;
        if ('a' <= *key && *key <= 'z') {
            idx = *key - 'a';
        } else if (*key == '_') {
            idx = 26;
        } else {
            spdlog::warn("invalid opt {}", key);
            return;
        }

        if (node->next_[idx] == nullptr) {
            node->next_[idx] = new TrieNode();
        }

        if (*(key + 1) == '\0') {
            node->next_[idx]->set_ = assign;
            node->is_flag_         = is_flag;
        }

        node = node->next_[idx];
    }
};

OptParse::TrieNode* OptParse::FindLongestMatch(const char* argv) {
    auto node = root_;
    for (; argv != nullptr && *argv != '\0'; ++argv) {
        int idx;
        if ('a' <= *argv && *argv <= 'z') {
            idx = *argv - 'a';
        } else if (*argv == '_') {
            idx = 26;
        } else {
            spdlog::warn("unkown argument {}", argv);
            break;
        }

        if (node == nullptr) {
            spdlog::warn("unkown argument {}", argv);
            break;
        }
        node = node->next_[idx];

        if (*(argv + 1) == '\0') {
            return node;
        }
    }
    return nullptr;
}

void OptParse::DestroyTrieTree(struct TrieNode* root) {
    if (root != nullptr) {
        for (auto node : root->next_) {
            DestroyTrieTree(node);
        }
        delete root->set_;
        delete root;
    }
}

void OptParse::AddOption(const char* opt, int* target) {
    if (opt != nullptr && target != nullptr) {
        auto assign = new std::function<void(const char*)>([target](const char* value) { *target = atoi(value); });
        InsertToTrie(opt, assign);
    }
}

void OptParse::AddOption(const char* opt, int* target, int defaults) {
    if (opt != nullptr && target != nullptr) {
        *target = defaults;
        AddOption(opt, target);
    }
};

void OptParse::AddOption(const char* opt, bool* target, bool defaults) {
    if (opt != nullptr && target != nullptr) {
        *target     = defaults;
        auto assign = new std::function<void(const char*)>([target, defaults](const char*) { *target = !defaults; });
        InsertToTrie(opt, assign, true);
    }
}

void OptParse::AddOption(const char* opt, char* target, size_t len) {
    if (opt != nullptr && target != nullptr) {
        auto assign =
            new std::function<void(const char*)>([target, len](const char* value) { strncpy(target, value, len); });
        InsertToTrie(opt, assign);
    }
}

void OptParse::AddOption(const char* opt, char* target, size_t len, const char* defaults) {
    if (opt != nullptr && target != nullptr) {
        if (defaults != nullptr) {
            std::strncpy(target, defaults, len);
        }
        AddOption(opt, target, len);
    }
}

void OptParse::Parse(int argc, const char** argv) {
    for (int i = 0; i < argc; ++i) {
        auto cur = argv[i];
        while (*cur != '\0' && *cur == '-') {
            ++cur;
        }
        auto node = FindLongestMatch(cur);
        if (node == nullptr) {
            return;
        }

        if (node->set_ != nullptr) {
            if (node->is_flag_) {
                (*node->set_)(cur);
            } else {
                if (i + 1 == argc) {
                    spdlog::error("no enough argument for option {}", argv[i]);
                    return;
                }
                (*node->set_)(argv[++i]);
            }
        }
    }
}

}  // namespace xserver