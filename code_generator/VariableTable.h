#include <vector>
#include <unordered_map>
#include <utility>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include "TypeInfoTable.h"

class VariableTable
{
private:
    std::vector<std::unordered_map<std::string, std::pair<llvm::AllocaInst *, TypeInfoClass *>>> variables_;

public:
    VariableTable(/* args */);
    void insert(std::string, llvm::AllocaInst *, TypeInfoClass *);
    std::pair<llvm::AllocaInst *, TypeInfoClass *> lookup(std::string);
    void beginScope();
    void endScope();
};
