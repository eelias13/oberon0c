//
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SEMANTICCHECKER_H
#define OBERON0C_SEMANTICCHECKER_H

#include "ScopeTable.h"
#include "util/Logger.h"
#include "parser/ast/NodeVisitor.h"
#include <limits>
#include <cmath>


class SemanticChecker : NodeVisitor {

    private:
        ScopeTable scope_table_;
        Logger& logger_;


    public:
        explicit SemanticChecker(Logger& logger);

        // Base Blocks/Empty Definitions
        void visit(IntNode&) override;
        void visit(UnaryExpressionNode&) override;
        void visit(BinaryExpressionNode&) override;
        void visit(IdentSelectorExpressionNode&) override;
        void visit(SelectorNode&) override;

        // Declarations
        void visit(ModuleNode&) override;
        void visit(ProcedureDeclarationNode&) override;
        void visit(DeclarationsNode&) override;

        // Type Nodes
        void visit(TypeNode&) override;
        void visit(ArrayTypeNode&) override;
        void visit(IdentNode&) override;
        void visit(RecordTypeNode&) override;

        // Statements
        void visit(StatementSequenceNode&) override;
        void visit(StatementNode&) override;
        void visit(AssignmentNode&) override;
        void visit(IfStatementNode&) override;
        void visit(RepeatStatementNode&) override;
        void visit(WhileStatementNode&) override;
        void visit(ProcedureCallNode&) override;

        // Record fields
        std::vector<std::pair<string,string>> key_value_map(RecordTypeNode&);

        // Typechecking
        string get_type_string(TypeNode&);
        string trace_type(const string& initial_type);

        string check_selector_type(IdentSelectorExpressionNode&);
        string check_selector_chain(IdentNode&, SelectorNode&);

        string checkType(ExpressionNode&);
        long evaluate_expression(ExpressionNode&, bool suppress_errors = false);

        void validate_program(ModuleNode&);

};


#endif //OBERON0C_SEMANTICCHECKER_H
