//
// Created by M on 15.12.2024.
//

#include "ScopeTable.h"

#include <utility>

void ScopeTable::beginScope()
{
    current_scope++;
    scopes_.emplace_back(std::make_unique<SymbolTable>());
}

void ScopeTable::endScope()
{
    current_scope--;
    scopes_.pop_back();
}

IdentInfo *ScopeTable::lookup(const string &name, bool only_current)
{

    if (only_current)
    {
        assert(current_scope >= 0);

        return scopes_[static_cast<size_t>(current_scope)]->lookup(name);
    }

    for (int i = current_scope; i >= 0; i--)
    {

        auto el = scopes_[static_cast<size_t>(i)]->lookup(name);
        if (el)
        {
            return el;
        }
    }

    return nullptr;
}

void ScopeTable::insert(const string &name, Kind k, const Node *node, string type)
{
    assert(current_scope >= 0);
    scopes_[static_cast<size_t>(current_scope)]->insert(name, k, node, std::move(type));
}

string ScopeTable::lookup_field(const string &record_name, const string &field_name)
{

    for (int i = current_scope; i >= 0; i--)
    {

        auto rec = scopes_[static_cast<size_t>(i)]->lookup_field(record_name, field_name);
        if (rec != "_ERROR")
        {
            return rec;
        }
    }

    return "_ERROR";
}

void ScopeTable::insert_record(const string &record_name, std::vector<std::pair<string, string>> fields)
{
    assert(current_scope >= 0);
    scopes_[static_cast<size_t>(current_scope)]->insert_record(record_name, std::move(fields));
}
