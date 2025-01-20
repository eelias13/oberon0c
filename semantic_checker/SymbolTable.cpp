//
// Created by M on 15.12.2024.
//

#include "SymbolTable.h"

#include <utility>

void SymbolTable::insert(const std::string &name, Kind k, Node *node, GeneralType general_type, string type)
{

    // Check if already inserted
    if (table_.find(name) != table_.end())
    {
        return;
    }

    table_[name] = IdentInfo(name, k, node, {general_type,std::move(type)});
}

void SymbolTable::insert_array_type(const string &name, Node *node, Type *element_type, int dimension) {
    if(table_.find(name) != table_.end()){
        return;
    }

    table_[name] = IdentInfo(name, Kind::TYPENAME, node, {ARRAY,"",dimension,std::make_shared<Type>(*element_type)});
}

IdentInfo *SymbolTable::lookup(const std::string &name)
{

    // return nullptr if not included
    auto node = table_.find(name);
    return (node != table_.end()) ? &node->second : nullptr;
}

void SymbolTable::insert_record(const string &record_name, std::vector<std::pair<string, Type>> fields)
{
    std::unordered_map<string, Type> field_map;
    for (auto itr = fields.begin(); itr != fields.end(); itr++)
    {
        field_map[itr->first] = itr->second;
    }

    records_[record_name] = field_map;

}

Type* SymbolTable::lookup_field(const string &record_name, const string &field_name)
{
    auto record = records_.find(record_name);
    if (record == records_.end())
    {
        return nullptr;
    }

    auto field = records_[record_name].find(field_name);
    if (field == records_[record_name].end())
    {
        return nullptr;
    }

    return &field->second;
}

void SymbolTable::insert(const string &name, Kind k, Node *node, Type type) {
    if(table_.find(name) != table_.end()){
        return;
    }

    table_[name] = IdentInfo(name,k, node,{type.general,type.name,type.array_dim,type.element_type});
}


bool Type::operator!=(Type other) {
    return !(*this == std::move(other));
}

bool Type::operator==(Type other) {

    switch(this->general){
        case BOOLEAN:
        case INTEGER:
            return (this->general == other.general);
        case ARRAY:
            return(other.general == ARRAY && this->array_dim==other.array_dim && this->element_type == other.element_type);
        case ALIAS:
            return(other.general == ALIAS && this->name == other.name);
        case RECORD:
        case ERROR_TYPE:
            return false;
    }
}
