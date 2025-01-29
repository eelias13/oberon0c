#include <vector>
#include <utility>
#include <string>
#include <unordered_map>
#include <llvm/IR/Type.h>

#ifndef OBERON0C_TYPEINFOTABLE_H
#define OBERON0C_TYPEINFOTABLE_H

enum TypeTag
{
    RECORD_TAG,
    ARRAY_TAG,
    INTEGER_TAG,
    BOOLEAN_TAG,
    // NO_TAG
};


// struct TypeInfoClass
// {
//     TypeTag tag;
//     std::vector<llvm::Type*> llvmType;
//     union Value
//     {
//         constexpr Value() : record(){};
//         constexpr ~Value(){};
//         constexpr Value(Value& v) { if(v.array.first == nullptr && v.array.second == -1){record = v.record;} else{array = v.array;}}
//         std::pair<TypeInfoClass *, int> array = {nullptr,-1};
//         std::vector<std::pair<std::string, TypeInfoClass *>> record;
//     } value;
// 
// };

struct TypeInfoClass
{
    TypeTag tag;
    std::vector<llvm::Type *> llvmType;
    struct Record
    {
        std::vector<std::pair<std::string, TypeInfoClass *>> fields;
    };
    struct Array
    {
        TypeInfoClass *elementType;
        int size;
    };
    std::variant<Record, Array> value;
};

class TypeInfoTable
{
private:
    std::vector<std::unordered_map<std::string, TypeInfoClass>> types_;

public:
    TypeInfoTable();
    void insert(std::string name, TypeInfoClass type_info);
    TypeInfoClass *lookup(std::string name);

    void beginScope();
    void endScope();
};

#endif //OBERON0_TYPEINFOTAG_H