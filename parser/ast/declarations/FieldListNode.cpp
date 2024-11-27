//
// Created by M on 19.11.2024.
//

#include "FieldListNode.h"
#include "IdentListNode.h"
#include "../base_blocks/IdentNode.h"
#include "TypeNode.h"

void FieldListNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void FieldListNode::print(ostream &stream) const
{

    if (fields_)
    {
        stream << *fields_ << " : " << *type_;
    }
}

FieldListNode::FieldListNode(FilePos pos) : Node(NodeType::field_list, pos), fields_(nullptr), type_(nullptr) {};

FieldListNode::FieldListNode(FilePos pos, std::unique_ptr<IdentListNode> fields, std::unique_ptr<TypeNode> type) : Node(NodeType::field_list, pos), fields_(std::move(fields)), type_(std::move(type)) {};
