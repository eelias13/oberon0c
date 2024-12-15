//
// Created by M on 15.12.2024.
//

#include "ScopeTable.h"

void ScopeTable::beginScope() {
    current_scope++;
    scopes_.emplace_back(std::make_unique<SymbolTable>());
}

void ScopeTable::endScope() {
    current_scope--;
    scopes_.pop_back();
}

const Node *ScopeTable::lookup(const string& name) {

    for(int i = current_scope; i >= 0; i--){

        const Node* ptr = scopes_[i]->lookup(name);
        if(ptr){
            return ptr;
        }

    }

    return nullptr;

}

void ScopeTable::insert(const string &name, const Node *node) {
    scopes_[current_scope]->insert(name,node);
}
