//
// Created by M on 19.11.2024.
//

#include "parser/ast/statements/ProcedureCallNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/base_blocks/ExpressionNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"
#include "parser/ast/NodeVisitor.h"

void ProcedureCallNode::accept(NodeVisitor &visitor)
{
    visitor.visit(*this);
}

void ProcedureCallNode::print(ostream &stream) const
{
    stream << *name_ << *selector_;

    if (parameters_)
    {
        stream << "(";

        for (auto itr = parameters_->begin(); itr != parameters_->end(); itr++)
        {
            if(itr > parameters_->begin()){
                stream << ", ";
            }

            stream << *(*itr);
        }

        stream << ")";

    }
}

ProcedureCallNode::ProcedureCallNode(FilePos pos, std::unique_ptr<IdentNode> name,std::unique_ptr<SelectorNode> selector, std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> parameters) : StatementNode(NodeType::procedure_call, pos), name_(std::move(name)), selector_(std::move(selector)), parameters_(std::move(parameters))  {}

