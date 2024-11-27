//
// Created by M on 19.11.2024.
//

#include "ActualParametersNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"

void ActualParametersNode::add_expression(std::unique_ptr<ExpressionNode> expr)
{
        expressions_.emplace_back(std::move(expr));
}

void ActualParametersNode::accept(NodeVisitor &visitor)
{
        (void)visitor;
}

void ActualParametersNode::print(ostream &stream) const
{
        stream << "(";

        for (auto itr = expressions_.begin(); itr != expressions_.end(); itr++)
        {
                stream << *(*itr);
        }

        stream << ")";
}

ActualParametersNode::ActualParametersNode(FilePos pos) : Node(NodeType::actual_parameters, pos) {};