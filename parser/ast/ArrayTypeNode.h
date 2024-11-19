//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_ARRAYTYPENODE_H
#define OBERON0C_ARRAYTYPENODE_H

#include "Node.h"


class ExpressionNode;
class TypeNode;

class ArrayTypeNode : Node {

    private:
        std::unique_ptr<ExpressionNode> name_;
        std::unique_ptr<TypeNode> type_;

    public:
        explicit ArrayTypeNode(const NodeType nodeType, FilePos pos, std::unique_ptr<ExpressionNode> name, std::unique_ptr<TypeNode> type) : Node(nodeType,pos),name_(std::move(name)), type_(std::move(type)) { };

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_ARRAYTYPENODE_H
