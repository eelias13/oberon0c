//
// Created by M on 21.11.2024.
//

#include "IdentNode.h"

void IdentNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void IdentNode::print(ostream &stream) const
{
    stream << name_;
}
