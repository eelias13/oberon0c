#ifndef OBERON0C_JSONVISITOR_H
#define OBERON0C_JSONVISITOR_H

#include "NodeVisitor.h"
#include "util/Logger.h"

class JsonVisitor : NodeVisitor
{
private:
    std::ostringstream json_;

public:
    explicit JsonVisitor() {}

    void visit(ExpressionNode &) override;
    void visit(BinaryExpressionNode &) override;
    void visit(UnaryExpressionNode &) override;
    void visit(IdentSelectorExpressionNode &) override;
    void visit(IdentNode &) override;
    void visit(IntNode &) override;
    void visit(SelectorNode &) override;
    void visit(TypeNode &) override;
    void visit(ArrayTypeNode &) override;
    void visit(DeclarationsNode &) override;
    void visit(ProcedureDeclarationNode &) override;
    void visit(RecordTypeNode &) override;
    void visit(StatementNode &) override;
    void visit(AssignmentNode &) override;
    void visit(IfStatementNode &) override;
    void visit(ProcedureCallNode &) override;
    void visit(RepeatStatementNode &) override;
    void visit(StatementSequenceNode &) override;
    void visit(WhileStatementNode &) override;
    void visit(ModuleNode &) override;

    std::string getJson();
};

#endif // OBERON0C_JSONVISITOR_H
