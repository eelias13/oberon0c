//
// Created by M on 19.11.2024.
//

#include "ArrayTypeNode.h"
#include "parser/ast/base_blocks/ExpressionNode.h"

void ArrayTypeNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ArrayTypeNode::print(ostream &stream) const
{
    stream << "ARRAY " << *name_ << " OF " << *type_;
}
