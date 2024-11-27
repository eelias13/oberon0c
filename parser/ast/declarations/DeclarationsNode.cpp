//
// Created by M on 21.11.2024.
//

#include "DeclarationsNode.h"
#include "ProcedureDeclarationNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/declarations/IdentListNode.h"

void DeclarationsNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void DeclarationsNode::print(ostream &stream) const
{

    if (!const_list_.empty())
    {
        stream << "CONST ";

        for (auto itr = const_list_.begin(); itr != const_list_.end(); itr++)
        {
            stream << *(itr->first) << " = " << *(itr->second) << "; ";
        }
    }

    if (!type_list_.empty())
    {
        stream << "TYPE ";

        for (auto itr = type_list_.begin(); itr != type_list_.end(); itr++)
        {
            stream << *(itr->first) << " = " << *(itr->second) << "; ";
        }
    }

    if (!var_list.empty())
    {
        stream << "VAR ";

        for (auto itr = var_list.begin(); itr != var_list.end(); itr++)
        {
            stream << *(itr->first) << " : " << *(itr->second) << "; ";
        }
    }

    for (auto itr = procedure_list.begin(); itr != procedure_list.end(); itr++)
    {
        stream << *(*itr) << "; ";
    }
}
