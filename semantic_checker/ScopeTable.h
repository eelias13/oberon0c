//
// Manages the Scopes of a Oberon0 Program
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SCOPETABLE_H
#define OBERON0C_SCOPETABLE_H

#include <vector>
#include <memory>
#include "SymbolTable.h"
#include "parser/ast/Node.h"

class ScopeTable {

    private:
        std::vector<std::unique_ptr<SymbolTable>> scopes_;
        int current_scope = -1;

    public:
        ScopeTable() = default;

        void beginScope();
        void endScope();
        const Node* lookup(const string& name);
        void insert(const string& name, const Node* node);

};


#endif //OBERON0C_SCOPETABLE_H
