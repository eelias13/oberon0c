//
// Created by M on 19.11.2024.
//

#include "IdentListNode.h"

void IdentListNode::accept(NodeVisitor &visitor) {

}

void IdentListNode::print(ostream &stream) const {

    for(auto itr = identifier_.begin(); itr != identifier_.end(); itr++){

        if(itr > identifier_.begin()){
            stream << ", ";
        }

        stream << itr->get();

    }
}
