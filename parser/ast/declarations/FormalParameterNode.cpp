//
// Created by M on 20.11.2024.
//

#include "FormalParameterNode.h"
#include "FPSectionNode.h"
#include "IdentListNode.h"
#include "parser/ast/base_blocks/IdentNode.h"

void FormalParameterNode::accept(NodeVisitor &visitor) {

}

void FormalParameterNode::print(ostream &stream) const {

    stream << "(";

    for(auto itr = parameter_sections_.begin(); itr != parameter_sections_.end(); itr++){

        if(itr > parameter_sections_.begin()){
            stream << "; ";
        }

        stream << *(*itr);

    }

    stream << ")";

}

FormalParameterNode::FormalParameterNode(FilePos pos)  : Node(NodeType::formal_parameters,pos){}

void FormalParameterNode::add_parameter_section(std::unique_ptr<FPSectionNode> section) {
    parameter_sections_.emplace_back(std::move(section));
};

