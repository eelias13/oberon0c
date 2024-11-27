//
// Created by M on 20.11.2024.
//

#include "ProcedureDeclarationNode.h"
#include "ProcedureHeadingNode.h"
#include "ProcedureBodyNode.h"
#include "DeclarationsNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "IdentListNode.h"
#include "parser/ast/statements/StatementSequenceNode.h"
#include "parser/ast/statements/StatementNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"
#include "FormalParameterNode.h"
#include "FPSectionNode.h"

void ProcedureDeclarationNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ProcedureDeclarationNode::print(ostream &stream) const
{
    stream << *heading_ << "; " << *body_;
}

ProcedureDeclarationNode::ProcedureDeclarationNode(FilePos pos, std::unique_ptr<ProcedureHeadingNode> heading, std::unique_ptr<ProcedureBodyNode> body) : Node(NodeType::procedure_declaration, pos), heading_(std::move(heading)), body_(std::move(body)) {};