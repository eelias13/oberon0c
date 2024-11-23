//
// Created by M on 20.11.2024.
//

#include "ProcedureDeclarationNode.h"

void ProcedureDeclarationNode::accept(NodeVisitor &visitor) {

}

void ProcedureDeclarationNode::print(ostream &stream) const {
    stream << *heading_ <<  "; " << *body_;
}
