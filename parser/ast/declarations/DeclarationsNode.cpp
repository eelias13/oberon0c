//
// Created by M on 21.11.2024.
//

#include "DeclarationsNode.h"
#include "ProcedureDeclarationNode.h"
#include "parser/ast/base_blocks/ExpressionNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/NodeVisitor.h"


void DeclarationsNode::accept(NodeVisitor &visitor)
{
    visitor.visit(*this);
}

void DeclarationsNode::print(ostream &stream) const
{

    if (!const_list_.empty())
    {
        stream << "CONST ";

        for (auto itr = const_list_.begin(); itr != const_list_.end(); itr++)
        {
            stream << *(itr->first) << " = " << *(itr->second) << ";\n\n";
        }
    }

    if (!type_list_.empty())
    {
        stream << "TYPE ";

        for (auto itr = type_list_.begin(); itr != type_list_.end(); itr++)
        {
            stream << *(itr->first) << " = " << *(itr->second) << ";\n\n";
        }
    }

    if (!var_list.empty())
    {
        stream << "VAR ";

        for (auto itr = var_list.begin(); itr != var_list.end(); itr++)
        {

            for(auto list_itr = itr->first->begin(); list_itr != itr->first->end(); list_itr++){
                if(list_itr > itr->first->begin()){
                    stream << ", ";
                }
                stream << *(*list_itr);
            }

            stream << " : " << *(itr->second) << ";\n\n";
        }
    }

    for (auto itr = procedure_list.begin(); itr != procedure_list.end(); itr++)
    {
        stream << *(*itr) << ";\n\n";
    }
}

std::vector<std::pair<string, ExpressionNode *>> DeclarationsNode::get_constants() {

    std::vector<std::pair<string, ExpressionNode *>> constants;
    for(auto itr = const_list_.begin(); itr != const_list_.end(); itr++){
        constants.emplace_back(itr->first->get_value(),itr->second.get());
    }

    return constants;

}

std::vector<std::pair<string, TypeNode *>> DeclarationsNode::get_typenames() {

    std::vector<std::pair<string, TypeNode *>> types;
    for(auto itr = type_list_.begin(); itr != type_list_.end(); itr++){
        types.emplace_back(itr->first->get_value(),itr->second.get());
    }

    return types;

}

std::vector<std::pair<std::vector<string>, TypeNode *>> DeclarationsNode::get_variables() {
    std::vector<std::pair<std::vector<string>, TypeNode *>> variables;

    for (auto itr = var_list.begin(); itr != var_list.end(); itr++){

        // Unpack variable names
        std::vector<string> typenames;
        for(auto id = itr->first->begin(); id != itr->first->end(); id++){
            typenames.emplace_back(id->get()->get_value());
        }

        variables.emplace_back(typenames,itr->second.get());
    }

    return variables;

}

std::vector<ProcedureDeclarationNode *> DeclarationsNode::get_procedures() {
    std::vector<ProcedureDeclarationNode*> procedures;

    for(auto itr = procedure_list.begin(); itr != procedure_list.end(); itr++){
        procedures.emplace_back(itr->get());
    }

    return procedures;
}
