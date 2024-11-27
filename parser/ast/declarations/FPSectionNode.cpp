//
// Created by M on 20.11.2024.
//

#include "FPSectionNode.h"
#include "IdentListNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "TypeNode.h"

void FPSectionNode::accept(NodeVisitor &visitor) {

}

void FPSectionNode::print(ostream &stream) const {
    if(var_included_){
        stream << "VAR ";
    }

    stream << *param_list_ << " : " << *type_;

}

FPSectionNode::FPSectionNode(FilePos pos, bool var_included, std::unique_ptr<IdentListNode> param_list, std::unique_ptr<TypeNode> type) : Node(NodeType::fp_section,pos), var_included_(var_included), param_list_(std::move(param_list)), type_(std::move(type)) {};

