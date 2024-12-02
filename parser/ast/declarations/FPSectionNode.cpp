//
// Created by M on 20.11.2024.
//

#include "FPSectionNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "TypeNode.h"

void FPSectionNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void FPSectionNode::print(ostream &stream) const
{
    if (var_included_)
    {
        stream << "VAR ";
    }

    for(auto itr = param_list_->begin(); itr != param_list_->end(); itr++){

        if(itr > param_list_->begin()){
            stream << ", ";
        }

        stream << *(*itr);

    }

    stream << " : " << *type_;
}

FPSectionNode::FPSectionNode(FilePos pos, bool var_included,std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> param_list,std::unique_ptr<TypeNode> type)  : Node(NodeType::fp_section, pos), var_included_(var_included), param_list_(std::move(param_list)), type_(std::move(type)) {};
