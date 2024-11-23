//
// Created by M on 22.11.2024.
//

#ifndef OBERON0C_FACTORNODE_H
#define OBERON0C_FACTORNODE_H

#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"

class ExpressionNode;

class FactorNode : Node {
    public:
        FactorNode(NodeType node_type, FilePos pos);

        virtual void accept(NodeVisitor &visitor) = 0;
        virtual void print(std::ostream &stream) const = 0;
};

class NegatedFactorNode : public FactorNode {

    private:
        std::unique_ptr<FactorNode> factor_;

    public:
        NegatedFactorNode(FilePos pos, std::unique_ptr<FactorNode> factor);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


class ExpressionInFactorNode : public FactorNode {
    private:
        std::unique_ptr<ExpressionNode> expr_;

    public:
        ExpressionInFactorNode(FilePos pos, std::unique_ptr<ExpressionNode> expr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


class IdentSelectorFactorNode : public FactorNode {
    private:
        std::unique_ptr<IdentNode> ident_;
        std::unique_ptr<SelectorNode> selector_;

    public:
        IdentSelectorFactorNode(FilePos pos, std::unique_ptr<IdentNode> ident, std::unique_ptr<SelectorNode> selector);
        ~IdentSelectorFactorNode() override = default;

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FACTORNODE_H
