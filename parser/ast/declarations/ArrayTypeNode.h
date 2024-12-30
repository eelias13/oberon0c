//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_ARRAYTYPENODE_H
#define OBERON0C_ARRAYTYPENODE_H

#include "TypeNode.h"
#include <memory>

class ExpressionNode;

class ArrayTypeNode : public TypeNode {

    private:
        std::unique_ptr<ExpressionNode> dim_;
        std::unique_ptr<TypeNode> type_;

    public:
        explicit ArrayTypeNode(FilePos pos, std::unique_ptr<ExpressionNode> dim, std::unique_ptr<TypeNode> type) : TypeNode(NodeType::array_type,pos),dim_(std::move(dim)), type_(std::move(type)) { };

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

        ExpressionNode* get_dimensions();
        TypeNode* get_type();
};


#endif //OBERON0C_ARRAYTYPENODE_H
