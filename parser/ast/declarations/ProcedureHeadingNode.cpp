//
// Created by M on 20.11.2024.
//

#include "ProcedureHeadingNode.h"
#include "parser/ast/base_blocks/IdentNode.h"

void ProcedureHeadingNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ProcedureHeadingNode::print(ostream &stream) const
{
    stream << "PROCEDURE " << *name_;

    if (params_)
    {

        stream << "(";

        // Print all FPSections
        for(auto itr = params_->begin();itr != params_->end(); itr++){

            if(itr > params_->begin()){
                stream << "; ";
            }

            // Print Single FPSection
            if(std::get<0>(*(*itr))){
                stream << "VAR ";
            }

            // Print all Parameters of FPSection
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
}

ProcedureHeadingNode::ProcedureHeadingNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<formal_parameters> params): Node(NodeType::procedure_heading, pos), name_(std::move(name)), params_(std::move(params)) {};


