//
// Created by M on 20.11.2024.
//

#include "ProcedureBodyNode.h"
#include "parser/ast/declarations/DeclarationsNode.h"
#include "parser/ast/declarations/ProcedureDeclarationNode.h"
#include "parser/ast/declarations/ProcedureHeadingNode.h"
#include "parser/ast/declarations/FormalParameterNode.h"
#include "parser/ast/declarations/FPSectionNode.h"
#include "parser/ast/statements/StatementSequenceNode.h"
#include "parser/ast/statements/StatementNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/declarations/IdentListNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"

void ProcedureBodyNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ProcedureBodyNode::print(ostream &stream) const
{
    stream << *declarations_;

    if (statements_)
    {
        stream << " BEGIN " << *statements_;
    }

    stream << " END " << *name_;
}

ProcedureBodyNode::ProcedureBodyNode(FilePos pos, std::unique_ptr<DeclarationsNode> declarations, std::unique_ptr<IdentNode> name, std::unique_ptr<StatementSequenceNode> statements)
    : Node(NodeType::procedure_body, pos), declarations_(std::move(declarations)), statements_(std::move(statements)), name_(std::move(name)) {};
