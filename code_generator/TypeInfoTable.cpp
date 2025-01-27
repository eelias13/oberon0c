#include "TypeInfo.h"

TypeInfoTable::TypeInfoTable()
{
    types_ = new std::unordered_map<std::string, TypeInfoClass>();
}
void TypeInfoTable::insert(std::string name, TypeInfoClass type_info)
{
    if (types_.find(name) != types_.end())
    {
        return;
    }
    types_[name] = type_info;
}
TypeInfoClass *TypeInfoTable::lookup(std::string name)
{
    if (types_.find(name) == types_.end())
    {
        return nullptr; // not found
    }
    return &types_[name];
}
