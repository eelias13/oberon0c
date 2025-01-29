#include "VariableTable.h"

void VariableTable::insert(std::string name, llvm::AllocaInst *var, TypeInfoClass *type, bool is_pointer)
{
    variables_.back().insert({name, {var, type, is_pointer}});
}
std::tuple<llvm::AllocaInst *, TypeInfoClass *, bool> VariableTable::lookup(std::string name)
{
    for (auto it = variables_.rbegin(); it != variables_.rend(); ++it)
    {
        if (it->find(name) != it->end())
        {
            return (*it)[name];
        }
    }

    return {nullptr, nullptr, false};
}
void VariableTable::beginScope()
{
    variables_.push_back(std::unordered_map<std::string, std::tuple<llvm::AllocaInst *, TypeInfoClass *, bool>>());
}
void VariableTable::endScope()
{
    variables_.pop_back();
}

VariableTable::VariableTable()
{
    variables_ = std::vector<std::unordered_map<std::string, std::tuple<llvm::AllocaInst *, TypeInfoClass *, bool>>>();
}
