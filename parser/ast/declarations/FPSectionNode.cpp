//
// Created by M on 20.11.2024.
//

#include "FPSectionNode.h"

void FPSectionNode::accept(NodeVisitor &visitor) {

}

void FPSectionNode::print(ostream &stream) const {
    if(var_included_){
        stream << "VAR ";
    }

    stream << *param_list_ << " : " << *type_;

}
