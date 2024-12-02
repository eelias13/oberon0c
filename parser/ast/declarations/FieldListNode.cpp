//
// Created by M on 19.11.2024.
//

#include "FieldListNode.h"
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
        for(auto itr = fields_->begin(); itr != fields_->end(); itr++){
            if(itr > fields_->begin()){
                stream << *(*itr);
            }
        }

        stream << " : " << *type_;
    }
}

FieldListNode::FieldListNode(FilePos pos) : Node(NodeType::field_list, pos), fields_(nullptr), type_(nullptr) {};

FieldListNode::FieldListNode(FilePos pos, std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> fields, std::unique_ptr<TypeNode> type) : Node(NodeType::field_list, pos), fields_(std::move(fields)), type_(std::move(type)) {};
