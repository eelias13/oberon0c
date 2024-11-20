//
// Created by M on 20.11.2024.
//

#include "FormalParameterNode.h"

void FormalParameterNode::accept(NodeVisitor &visitor) {

}

void FormalParameterNode::print(ostream &stream) const {

    stream << "(";

    for(auto itr = parameter_sections_.begin(); itr != parameter_sections_.end(); itr++){

        if(itr > parameter_sections_.begin()){
            stream << "; ";
        }

        stream << itr->get();

    }

    stream << ")";

}
