//
// Created by M on 22.11.2024.
//

#include "SimpleExpressionNode.h"

const string SimpleExpressionNode::op_to_string[] = {"+","-","OR","<NONE>"};

void SimpleExpressionNode::accept(NodeVisitor &visitor) {

}

void SimpleExpressionNode::print(ostream &stream) const {

    for(auto itr = expressions_.begin(); itr != expressions_.end(); itr++){

        if(itr->first != NONE){
            stream << " " << op_to_string[itr->first] << " ";
        }

        stream << itr->second.get();

    }

}
