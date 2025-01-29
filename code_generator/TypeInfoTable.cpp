#include "TypeInfo.h"

TypeInfoTable::TypeInfoTable()
{
    types_ = new std::unordered_map<std::string, TypeInfoClass>();
}
void TypeInfoTable::insert(std::string name, TypeInfoClass type_info)
{
    types_.back().insert(name, type_info);
}
TypeInfoClass *TypeInfoTable::lookup(std::string name)
{

    for (auto it = types_.rbegin(); it != variables_.rend(); ++it)
    {
        if (it->find(name))
        {
            return *it[name];
        }
    }

    return nullptr;
}

void TypeInfoTable::beginScope()
{
    types_.push_back(new std::unordered_map<std::string, TypeInfoClass>());
}
void TypeInfoTable::endScope()
{
    types_.pop_back();
}