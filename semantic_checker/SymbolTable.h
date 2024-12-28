//
// Manages all Identifiers within a scope of a Oberon0 Program
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SYMBOLTABLE_H
#define OBERON0C_SYMBOLTABLE_H

#include <unordered_map>
#include "parser/ast/Node.h"

enum Kind {PROCEDURE, CONSTANT, VARIABLE, TYPENAME, ERROR_KIND};

struct IdentInfo {
    Kind kind;
    const Node* node;
    string type;
};

class SymbolTable {

    private:
        std::unordered_map<string, IdentInfo> table_;

    public:
        explicit SymbolTable() = default;

        void insert(const string& name, Kind k, const Node* node, string type = "");
        IdentInfo* lookup(const std::string &name);

};


#endif //OBERON0C_SYMBOLTABLE_H
