//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_ARRAYTYPENODE_H
#define OBERON0C_ARRAYTYPENODE_H

#include "TypeNode.h"
#include "ExpressionNode.h"

class ArrayTypeNode : TypeNode {

    private:
        std::unique_ptr<ExpressionNode> name_;
        std::unique_ptr<TypeNode> type_;

    public:
        explicit ArrayTypeNode(FilePos pos, std::unique_ptr<ExpressionNode> name, std::unique_ptr<TypeNode> type) : TypeNode(NodeType::array_type,pos),name_(std::move(name)), type_(std::move(type)) { };

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_ARRAYTYPENODE_H
