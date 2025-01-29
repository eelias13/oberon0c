#include <vector>
#include <unordered_map>
#include <utility>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include "TypeInfoTable.h"

class VariableTable
{
private:
    std::vector<std::unordered_map<std::string, std::tuple<llvm::AllocaInst *, TypeInfoClass *, bool >>> variables_;

public:
    VariableTable(/* args */);
    void insert(std::string, llvm::AllocaInst *, TypeInfoClass *, bool is_pointer = false);
    std::tuple<llvm::AllocaInst *, TypeInfoClass *, bool> lookup(std::string);
    void beginScope();
    void endScope();
};
