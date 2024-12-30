//
// Created by M on 15.12.2024.
//

#include "ScopeTable.h"

#include <utility>

void ScopeTable::beginScope() {
    current_scope++;
    scopes_.emplace_back(std::make_unique<SymbolTable>());
}

void ScopeTable::endScope() {
    current_scope--;
    scopes_.pop_back();
}

IdentInfo* ScopeTable::lookup(const string& name, bool only_current) {

    if(only_current){
        return scopes_[current_scope]->lookup(name);
    }

    for(int i = current_scope; i >= 0; i--){

        auto el = scopes_[i]->lookup(name);
        if(el){
            return el;
        }

    }

    return nullptr;

}

void ScopeTable::insert(const string &name, Kind k, const Node *node, string type) {
    scopes_[current_scope]->insert(name,k,node,std::move(type));
}

string ScopeTable::lookup_field(const string &record_name, const string &field_name) {
    for(int i = current_scope; i >= 0; i--){

        auto rec = scopes_[i]->lookup_field(record_name,field_name);
        if(rec != "_ERROR"){
            return rec;
        }

    }

    return "_ERROR";
}

void ScopeTable::insert_record(const string &record_name, std::vector<std::pair<string, string>> fields) {
    scopes_[current_scope]->insert_record(record_name,std::move(fields));
}
