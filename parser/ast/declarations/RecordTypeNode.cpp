//
// Created by M on 20.11.2024.
//

#include "RecordTypeNode.h"

void RecordTypeNode::accept(NodeVisitor &visitor) {

}

void RecordTypeNode::print(ostream &stream) const {
    stream << "RECORD ";

    for(auto itr = fields_.begin(); itr != fields_.end(); itr++){

        if(itr > fields_.begin()){
            stream << "; ";
        }
        stream << itr->get();

    }

    stream << " END ";

}
