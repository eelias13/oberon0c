//
// Created by M on 20.11.2024.
//

#include "FormalParameterNode.h"
#include "parser/ast/base_blocks/IdentNode.h"

void FormalParameterNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void FormalParameterNode::print(ostream &stream) const
{

    stream << "(";

    for (auto itr = parameter_sections_.begin(); itr != parameter_sections_.end(); itr++)
    {

        if (itr > parameter_sections_.begin())
        {
            stream << "; ";
        }

        // Print FPSection
        if(std::get<0>(*(*itr))){
            stream << "VAR ";
        }

        for(auto param_list_itr = (*std::get<1>(*(*itr))).begin(); param_list_itr != (*std::get<1>(*(*itr))).end(); param_list_itr++){

            if(param_list_itr > (*std::get<1>(*(*itr))).begin()){
                stream << ", ";
            }

            stream << *(*param_list_itr);

        }

        stream << " : " << *(std::get<2>(*(*itr)));

    }

    stream << ")";
}

FormalParameterNode::FormalParameterNode(FilePos pos) : Node(NodeType::formal_parameters, pos) {}

void FormalParameterNode::add_parameter_section(std::unique_ptr<fp_section> section)
{
    parameter_sections_.emplace_back(std::move(section));
};
