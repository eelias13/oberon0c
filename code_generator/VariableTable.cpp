#include "VariableTable.h"

void VariableTable::insert(std::string name, llvm::AllocaInst *var, TypeInfoClass *type)
{
    variables_.back().insert({name, {var, type}});
}
std::pair<llvm::AllocaInst *, TypeInfoClass *> VariableTable::lookup(std::string name)
{
    for (auto it = variables_.rbegin(); it != variables_.rend(); ++it)
    {
        if (it->find(name) != it->end())
        {
            return (*it)[name];
        }
    }

    return {nullptr, nullptr};
}
void VariableTable::beginScope()
{
    variables_.push_back(std::unordered_map<std::string, std::pair<llvm::AllocaInst *, TypeInfoClass *>>());
}
void VariableTable::endScope()
{
    variables_.pop_back();
}

VariableTable::VariableTable()
{
    variables_ = std::vector<std::unordered_map<std::string, std::pair<llvm::AllocaInst *, TypeInfoClass *>>>();
}
