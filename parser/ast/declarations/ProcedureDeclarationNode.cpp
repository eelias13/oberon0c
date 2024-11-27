//
// Created by M on 20.11.2024.
//

#include "ProcedureDeclarationNode.h"
#include "ProcedureHeadingNode.h"
#include "ProcedureBodyNode.h"


void ProcedureDeclarationNode::accept(NodeVisitor &visitor) {

}

void ProcedureDeclarationNode::print(ostream &stream) const {
    stream << *heading_ <<  "; " << *body_;
}

ProcedureDeclarationNode::ProcedureDeclarationNode(FilePos pos, std::unique_ptr<ProcedureHeadingNode> heading, std::unique_ptr<ProcedureBodyNode> body)  : Node(NodeType::procedure_declaration,pos),heading_(std::move(heading)),body_(std::move(body)) {};