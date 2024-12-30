//
// Created by M on 19.11.2024.
//

#include "ArrayTypeNode.h"
#include "parser/ast/base_blocks/ExpressionNode.h"
#include "parser/ast/NodeVisitor.h"


void ArrayTypeNode::accept(NodeVisitor &visitor)
{
    visitor.visit(*this);
}

void ArrayTypeNode::print(ostream &stream) const
{
    stream << "ARRAY " << *dim_ << " OF " << *type_;
}

ExpressionNode *ArrayTypeNode::get_dimensions() {
    return dim_.get();
}

TypeNode *ArrayTypeNode::get_type() {
    return type_.get();
}
