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

void IdentNode::set_more_types_yay(TypeInfo more_types_yay)
{
    more_types_yay_ = more_types_yay;
}
std::optional<TypeInfo> IdentNode::get_more_types_yay(){
    return more_types_yay_; 
    }

string IdentNode::get_value()
{
    return name_;
}

void IdentNode::set_types(TypeInfo formal, TypeInfo actual, TypeNode *node)
{
    formal_type_ = std::move(formal);
    actual_type_ = std::move(actual);
    type_node_ = node;
}

TypeInfo IdentNode::get_formal_type()
{
    return formal_type_;
}

TypeNode *IdentNode::get_type_node()
{
    return type_node_;
}

TypeInfo IdentNode::get_actual_type()
{
    return actual_type_;
}
