//
// Created by M on 21.11.2024.
//

#include "SelectorNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"
#include "parser/ast/base_blocks/IdentNode.h"

void SelectorNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void SelectorNode::print(ostream &stream) const
{

    for (auto itr = selectors.begin(); itr != selectors.end(); itr++)
    {

        if (std::get<0>(*itr))
        {

            // Array Index
            stream << "[" << *std::get<2>(*itr) << "]";
        }
        else
        {

            // Field
            stream << "." << *std::get<1>(*itr);
        }
    }
}

void SelectorNode::add_field(std::unique_ptr<IdentNode> ident)
{
    selectors.emplace_back(false, std::move(ident), nullptr);
}

void SelectorNode::add_index(std::unique_ptr<ExpressionNode> expr)
{
    selectors.emplace_back(true, nullptr, std::move(expr));
}

SelectorNode::SelectorNode(FilePos pos) : Node(NodeType::selector, pos)
{
}
