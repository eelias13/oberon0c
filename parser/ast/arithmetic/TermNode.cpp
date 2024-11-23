//
// Created by M on 22.11.2024.
//

#include "TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"

const string TermNode::op_to_string[] = {"*", "DIV", "MOD", "&","<NONE>"};

void TermNode::accept(NodeVisitor &visitor) {

}

void TermNode::print(ostream &stream) const {
    for(auto itr = factors_.begin(); itr != factors_.end(); itr++){

        if(itr->first != TERM_NONE){
            stream << " " << op_to_string[itr->first] << " ";
        }

        stream << *(itr->second);

    }
}

TermNode::TermNode(FilePos pos, std::unique_ptr<FactorNode> first_factor) : Node(NodeType::term,pos) {
    factors_.emplace_back(TERM_NONE, std::move(first_factor));
}

void TermNode::addFactor(term_ops op, std::unique_ptr<FactorNode> factor) {
    factors_.emplace_back(op,std::move(factor));
}
