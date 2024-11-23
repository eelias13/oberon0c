//
// Created by M on 18.11.2024.
//

#include "StatementSequenceNode.h"

void StatementSequenceNode::accept(NodeVisitor &visitor) {

}

void StatementSequenceNode::print(ostream &stream) const {
    for(auto itr = statements_.begin(); itr != statements_.end(); itr++){

        if(itr > statements_.begin()){
            stream << "; ";
        }

        stream << *(*itr);

    }
}
