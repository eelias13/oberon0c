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

    table_[name] = IdentInfo(name,k,node,std::move(type));

}

IdentInfo* SymbolTable::lookup(const std::string &name) {

    // return nullptr if not included
    auto node = table_.find(name);
    return (node != table_.end()) ? &node->second : nullptr;

}

void SymbolTable::insert_record(const string &record_name, std::vector<std::pair<string, string>> fields) {

    std::unordered_map<string,string> field_map;
    for(auto itr = fields.begin(); itr != fields.end(); itr++){
        field_map[itr->first] = itr->second;
    }

    records_[record_name] = field_map;
}

string SymbolTable::lookup_field(const string &record_name, const string &field_name) {

    auto record = records_.find(record_name);
    if(record == records_.end()){
        cerr << "Record '" << record_name << "' not found in records list";
        return "_ERROR";
    }

    auto field = records_[record_name].find(field_name);
    if(record == records_.end()){
        return "_ERROR";
    }

    return field->second;

}
