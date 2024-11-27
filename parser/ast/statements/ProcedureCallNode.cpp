//
// Created by M on 19.11.2024.
//

#include "parser/ast/statements/ProcedureCallNode.h"
#include "parser/ast/statements/ActualParametersNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"

void ProcedureCallNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ProcedureCallNode::print(ostream &stream) const
{
    stream << *name_ << *selector_;

    if (parameters_)
    {
        stream << *parameters_;
    }
}

ProcedureCallNode::ProcedureCallNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<SelectorNode> selector, std::unique_ptr<ActualParametersNode> parameters) : StatementNode(NodeType::procedure_call, pos), name_(std::move(name)), selector_(std::move(selector)), parameters_(std::move(parameters)) {};
