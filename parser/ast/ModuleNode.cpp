//
// Created by M on 20.11.2024.
//

#include "ModuleNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/declarations/DeclarationsNode.h"
#include "parser/ast/declarations/ProcedureDeclarationNode.h"
#include "parser/ast/declarations/ProcedureHeadingNode.h"
#include "parser/ast/declarations/FormalParameterNode.h"
#include "parser/ast/declarations/ProcedureBodyNode.h"

#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"
#include "parser/ast/statements/StatementSequenceNode.h"
#include "parser/ast/statements/StatementNode.h"

void ModuleNode::accept(NodeVisitor &visitor)
{
    (void)visitor;
}

void ModuleNode::print(ostream &stream) const
{
    stream << "MODULE " << *module_name_begin_ << " ; " << *declarations_;

    if (statements_)
    {
        stream << *statements_;
    }

    stream << " END " << *module_name_end_ << ".";
}

ModuleNode::ModuleNode(FilePos pos, std::unique_ptr<IdentNode> name_start, std::unique_ptr<DeclarationsNode> declarations, std::unique_ptr<StatementSequenceNode> statements, std::unique_ptr<IdentNode> name_end) : Node(NodeType::module, pos), module_name_begin_(std::move(name_start)), declarations_(std::move(declarations)), statements_(std::move(statements)), module_name_end_(std::move(name_end)) {};
