//
// Created by M on 15.12.2024.
//

#include "SymbolTable.h"

#include <utility>

void SymbolTable::insert(const std::string &name, Kind k, const Node *node, string type) {

    // Check if already inserted
    if(table_.find(name) != table_.end()){
        cerr << "Tried to insert the same name into symbol table multiple times!";
        return;
    }

    table_[name] = IdentInfo(k,node,std::move(type));

}

IdentInfo* SymbolTable::lookup(const std::string &name) {

    // return nullptr if not included
    auto node = table_.find(name);
    return (node != table_.end()) ? &node->second : nullptr;

}