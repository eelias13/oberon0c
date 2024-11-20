//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREBODYNODE_H
#define OBERON0C_PROCEDUREBODYNODE_H

#include "StatementSequenceNode.h"

class DeclarationsNode;
class IdentNode;

class ProcedureBodyNode : Node {

    private:

        std::unique_ptr<DeclarationsNode> declarations_;
        std::unique_ptr<StatementSequenceNode> statements_;
        std::unique_ptr<IdentNode> name_;

    public:

        ProcedureBodyNode(const NodeType node_type, FilePos pos, std::unique_ptr<DeclarationsNode> declarations, std::unique_ptr<IdentNode> name, std::unique_ptr<StatementSequenceNode> statements = nullptr) :
                        Node(node_type,pos), declarations_(std::move(declarations)), statements_(std::move(statements)), name_(std::move(name)) {};


        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_PROCEDUREBODYNODE_H
