//
// Created by M on 18.11.2024.
//

#include "WhileStatementNode.h"
#include "parser/ast/statements/StatementSequenceNode.h"
#include "parser/ast/base_blocks/ExpressionNode.h"

void WhileStatementNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void WhileStatementNode::print(ostream &stream) const
{

    stream << "WHILE " << *condition_ << " DO " << *statements_ << " END";
}

WhileStatementNode::WhileStatementNode(FilePos pos, std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementSequenceNode> statements) : StatementNode(NodeType::while_statement, pos), condition_(std::move(condition)), statements_(std::move(statements)) {};
