//
// Created by M on 20.11.2024.
//

#include "ProcedureBodyNode.h"
#include "parser/ast/declarations/DeclarationsNode.h"

void ProcedureBodyNode::accept(NodeVisitor &visitor) {

}

void ProcedureBodyNode::print(ostream &stream) const {
    stream << declarations_;

    if(statements_){
        stream << " BEGIN " << statements_;
    }

    stream << " END " << name_;

}

ProcedureBodyNode::ProcedureBodyNode(FilePos pos, std::unique_ptr<DeclarationsNode> declarations, std::unique_ptr<IdentNode> name, std::unique_ptr<StatementSequenceNode> statements)
: Node(NodeType::procedure_body,pos), declarations_(std::move(declarations)), statements_(std::move(statements)), name_(std::move(name)) {};

