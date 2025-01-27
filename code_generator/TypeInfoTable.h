#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

enum TypeTag
{
    RECORD,
    ARRAY,
    INTEGER,
    BOOLEAN,
}

struct TypeInfoClass
{
    TypeTag tag;
    std::vector<llvm::Type *> llvmType;
    union Value
    {
        std::pair<TypeInfoClass *, int> array;
        std::vector<std::pair<std::string, TypeInfoClass *>> record;
    } value;
}

class TypeInfoTable
{
private:
    std::unordered_map<std::string, TypeInfoClass> types_;

public:
    TypeInfoTable();
    void insert(std::string name, TypeInfoClass type_info);
    TypeInfoClass *lookup(std::string name);
};
