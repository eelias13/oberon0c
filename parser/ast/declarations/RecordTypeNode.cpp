//
// Created by M on 20.11.2024.
//

#include "RecordTypeNode.h"
#include "parser/ast/declarations/FieldListNode.h"
#include "IdentListNode.h"
#include "parser/ast/base_blocks/IdentNode.h"

void RecordTypeNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void RecordTypeNode::print(ostream &stream) const
{
    stream << "RECORD ";

    for (auto itr = fields_.begin(); itr != fields_.end(); itr++)
    {

        if (itr > fields_.begin())
        {
            stream << "; ";
        }
        stream << (*itr);
    }

    stream << " END ";
}

RecordTypeNode::RecordTypeNode(FilePos pos, std::unique_ptr<FieldListNode> first_field) : TypeNode(NodeType::record_type, pos) { fields_.emplace_back(std::move(first_field)); };