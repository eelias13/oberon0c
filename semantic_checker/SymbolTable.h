//
// Manages all Identifiers within a scope of a Oberon0 Program
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SYMBOLTABLE_H
#define OBERON0C_SYMBOLTABLE_H

#include <unordered_map>
#include "parser/ast/Node.h"

class SymbolTable {

    private:
        std::unordered_map<string,const Node*> table_;

    public:
        explicit SymbolTable() = default;

        void insert(const string& name, const Node* node);
        const Node* lookup(const std::string &name);

};


#endif //OBERON0C_SYMBOLTABLE_H
