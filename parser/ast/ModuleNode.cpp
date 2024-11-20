//
// Created by M on 20.11.2024.
//

#include "ModuleNode.h"

void ModuleNode::accept(NodeVisitor &visitor) {

}

void ModuleNode::print(ostream &stream) const {
    stream << "MODULE " << module_name_begin_ << " ; " << declarations_;

    if(statements_){
        stream << statements_;
    }

    stream << " END " << module_name_end_ << ".";

}
