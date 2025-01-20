//
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SEMANTICCHECKER_H
#define OBERON0C_SEMANTICCHECKER_H

#include "ScopeTable.h"
#include "util/Logger.h"
#include "parser/ast/NodeVisitor.h"
#include <limits>
#include <optional>
#include <cmath>


class SemanticChecker : NodeVisitor {

    private:
        ScopeTable scope_table_;
        Logger& logger_;

        inline static const Type error_type = {ERROR_TYPE, ""};
        inline static const Type boolean_type = {BOOLEAN, "BOOLEAN"};
        inline static const Type integer_type = {INTEGER, "INTEGER"};


    public:
        explicit SemanticChecker(Logger& logger);

        // Base Blocks/Empty Definitions
        void visit(IntNode&) override;
        void visit(ExpressionNode&) override;
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
        std::vector<std::pair<string,Type>> key_value_map(RecordTypeNode&);

        // Typechecking
        Type get_type(TypeNode&, const string&);
        Type trace_type(Type initial_type);

        Type check_selector_type(IdentSelectorExpressionNode&);
        Type check_selector_chain(IdentNode&, SelectorNode&);

        Type checkType(ExpressionNode&);
        std::optional<long> evaluate_expression(ExpressionNode&, bool suppress_errors = false);

        void validate_program(ModuleNode&);

};


#endif //OBERON0C_SEMANTICCHECKER_H
