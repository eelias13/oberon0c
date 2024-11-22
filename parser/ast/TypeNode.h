//
// Created by M on 22.11.2024.
//

#ifndef OBERON0C_TYPENODE_H
#define OBERON0C_TYPENODE_H

#include "Node.h"

class TypeNode : Node {

    public:
    TypeNode(NodeType node_type, FilePos pos) : Node(node_type,pos) {};
    virtual ~TypeNode() = 0;

    virtual void accept(NodeVisitor &visitor) = 0;
    virtual void print(std::ostream &stream) const = 0;


};

#endif //OBERON0C_TYPENODE_H
