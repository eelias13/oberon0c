//
// Created by M on 20.11.2024.
//

#include "RecordTypeNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/NodeVisitor.h"

void RecordTypeNode::accept(NodeVisitor &visitor)
{
    visitor.visit(*this);
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

        // Print single Field List
        auto curr_field_list = itr->get()->first.get();
        for(auto field_itr = curr_field_list->begin(); field_itr != curr_field_list->end(); field_itr++){

            if(field_itr > curr_field_list->begin()){
                stream << ", ";
            }

            stream << *(*field_itr);

        }

        stream << " : " << *(itr->get()->second);

    }

    stream << " END\n";
}

RecordTypeNode::RecordTypeNode(FilePos pos, std::unique_ptr<field > first_field) : TypeNode(NodeType::record_type, pos) { fields_.emplace_back(std::move(first_field)); }

void RecordTypeNode::add_field_list(std::unique_ptr<field> field_list) {fields_.emplace_back(std::move(field_list));}