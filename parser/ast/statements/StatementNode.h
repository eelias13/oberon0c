//
// Created by M on 22.11.2024.
//

#ifndef OBERON0C_STATEMENTNODE_H
#define OBERON0C_STATEMENTNODE_H

#include "parser/ast/Node.h"

class StatementNode : public Node {

    public:
        StatementNode(NodeType node_type, FilePos pos) : Node(node_type,pos) {};
        virtual ~StatementNode() = 0;

        virtual void accept(NodeVisitor &visitor) = 0;
        virtual void print(std::ostream &stream) const = 0;

};


#endif //OBERON0C_STATEMENTNODE_H
