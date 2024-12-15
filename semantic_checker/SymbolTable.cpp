//
// Created by M on 15.12.2024.
//

#include "SymbolTable.h"

void SymbolTable::insert(const std::string &name, const Node *node) {

    // Check if already inserted
    table_[name] = node;

}

const Node *SymbolTable::lookup(const std::string &name) {

    // return nullptr if not included
    auto node = table_.find(name);
    return (node != table_.end()) ? node->second : nullptr;

}