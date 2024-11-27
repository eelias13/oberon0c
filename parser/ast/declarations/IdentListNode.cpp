//
// Created by M on 19.11.2024.
//

#include "IdentListNode.h"
#include "parser/ast/base_blocks/IdentNode.h"

void IdentListNode::accept(NodeVisitor &visitor) {

}

void IdentListNode::print(ostream &stream) const {

    for(auto itr = identifier_.begin(); itr != identifier_.end(); itr++){

        if(itr > identifier_.begin()){
            stream << ", ";
        }

        stream << *(*itr);

    }
}

void IdentListNode::add_identifier(std::unique_ptr<IdentNode> ident) {
    identifier_.emplace_back(std::move(ident));
}

IdentListNode::IdentListNode(FilePos pos, std::unique_ptr<IdentNode> first_identifier) : Node(NodeType::ident_list,pos) { identifier_.emplace_back(std::move(first_identifier));};
