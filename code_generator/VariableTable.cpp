
void VariableTable::insert(std::string name, llvm::AllocaInst *var, TypeInfoClass *type)
{
    variables_.back().insert(name, (var, type));
}
std::pair<llvm::AllocaInst *, TypeInfoClass *> VariableTable::lookup(std::string name)
{
    for (auto it = variables_.rbegin(); it != variables_.rend(); ++it)
    {
        if (it->find(name))
        {
            return *it[name];
        }
    }

    return nullptr;
}
void VariableTable::beginScope()
{
    variables_.push_back(new std::unordered_map<std::string, std::pair<llvm::AllocaInst *, TypeInfoClass *>>());
}
void VariableTable::endScope()
{
    variables_.pop_back();
}

VariableTable::VariableTable()
{
    variables_ = new std::vector<std::unordered_map<std::string, std::pair<llvm::AllocaInst *, TypeInfoClass *>>>()
}
