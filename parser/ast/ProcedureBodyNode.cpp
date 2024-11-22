//
// Created by M on 20.11.2024.
//

#include "ProcedureBodyNode.h"
#include "DeclarationsNode.h"

void ProcedureBodyNode::accept(NodeVisitor &visitor) {

}

void ProcedureBodyNode::print(ostream &stream) const {
    stream << declarations_;

    if(statements_){
        stream << " BEGIN " << statements_;
    }

    stream << " END " << name_;

}
