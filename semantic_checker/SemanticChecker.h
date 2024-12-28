//
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SEMANTICCHECKER_H
#define OBERON0C_SEMANTICCHECKER_H

#include "ScopeTable.h"
#include "util/Logger.h"
#include "parser/ast/NodeVisitor.h"

enum expression_type {INT, BOOLEAN, ERR_TYPE};

class SemanticChecker : NodeVisitor {

    private:
        ScopeTable scope_table_;
        Logger& logger_;


    public:
        explicit SemanticChecker(Logger& logger);

        void visit(ModuleNode&) override;
        void visit(ProcedureDeclarationNode&) override;
        void visit(DeclarationsNode&) override;

        string checkType(ExpressionNode&);
        long evaluate_expression(ExpressionNode&);

};


#endif //OBERON0C_SEMANTICCHECKER_H
