//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_STATEMENTSEQUENCENODE_H
#define OBERON0C_STATEMENTSEQUENCENODE_H

#include "Node.h"
#include <vector>

class StatementNode;

class StatementSequenceNode : public Node{

    private:
        std::vector<std::unique_ptr<StatementNode>> statements_;

    public:
        // StatementSequenceNode(const NodeType nodeType, FilePos pos,std::unique_ptr<StatementNode> first_statement) : Node(nodeType, pos) {statements_.emplace_back(std::move(first_statement));}

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_STATEMENTSEQUENCENODE_H
