//
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SEMANTICCHECKER_H
#define OBERON0C_SEMANTICCHECKER_H

#include "ScopeTable.h"
#include "util/Logger.h"
#include "parser/ast/NodeVisitor.h"

class SemanticChecker : NodeVisitor {

    private:
        ScopeTable scope_table_;
        Logger& logger_;


    public:
        explicit SemanticChecker(Logger& logger);

        // Base Blocks
        //void visit(IdentNode&) override;
        //void visit(IntNode&) override;

        // Declarations
        void visit(ModuleNode&) override;
        void visit(ProcedureDeclarationNode&) override;
        void visit(DeclarationsNode&) override;

        // Statements


        // Typechecking
        string get_type_string(TypeNode&);
        string trace_type(const string& initial_type);

        string checkSelectorType(IdentSelectorExpressionNode&);
        string checkSelectorChain(IdentNode&, SelectorNode&);

        string checkType(ExpressionNode&);
        long evaluate_expression(ExpressionNode&);

};


#endif //OBERON0C_SEMANTICCHECKER_H
