//
// Created by M on 21.11.2024.
//

#include "IdentNode.h"

#include <utility>
#include "parser/ast/NodeVisitor.h"


void IdentNode::accept(NodeVisitor &visitor)
{
    visitor.visit(*this);
}

void IdentNode::print(ostream &stream) const
{
    stream << name_;
}

string IdentNode::get_value() {
    return name_;
}

void IdentNode::set_types(string formal, TypeNode *node) {
    formal_type_ = std::move(formal);
    type_node_ = node;
}

string IdentNode::get_formal_type() {
    return formal_type_;
}

TypeNode *IdentNode::get_type_node() {
    return type_node_;
}
