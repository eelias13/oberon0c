//
// Created by M on 22.11.2024.
//

#include "FactorNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"

void NegatedFactorNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void NegatedFactorNode::print(ostream &stream) const
{
    stream << "~" << *factor_;
}

NegatedFactorNode::NegatedFactorNode(FilePos pos, std::unique_ptr<FactorNode> factor)
    : FactorNode(NodeType::factor, pos), factor_(std::move(factor)) {};

void ExpressionInFactorNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ExpressionInFactorNode::print(ostream &stream) const
{
    stream << "(" << *expr_ << ")";
}

ExpressionInFactorNode::ExpressionInFactorNode(FilePos pos, std::unique_ptr<ExpressionNode> expr) : FactorNode(NodeType::factor, pos), expr_(std::move(expr)) {};

void IdentSelectorFactorNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void IdentSelectorFactorNode::print(ostream &stream) const
{
    stream << *ident_ << *selector_;
}

IdentSelectorFactorNode::IdentSelectorFactorNode(FilePos pos, std::unique_ptr<IdentNode> ident, std::unique_ptr<SelectorNode> selector)
    : FactorNode(NodeType::factor, pos), ident_(std::move(ident)), selector_(std::move(selector)) {};

FactorNode::FactorNode(NodeType node_type, FilePos pos) : Node(node_type, pos) {};