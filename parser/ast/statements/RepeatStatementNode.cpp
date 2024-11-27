//
// Created by M on 18.11.2024.
//

#include "RepeatStatementNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"
#include "StatementSequenceNode.h"

void RepeatStatementNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void RepeatStatementNode::print(ostream &stream) const
{
    stream << "REPEAT " << *statements_ << " UNTIL " << *condition_;
}

RepeatStatementNode::RepeatStatementNode(FilePos pos, std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementSequenceNode> statements) : StatementNode(NodeType::repeat_statement, pos), condition_(std::move(condition)), statements_(std::move(statements)) {};
