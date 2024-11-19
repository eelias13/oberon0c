//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_ACTUALPARAMETERSNODE_H
#define OBERON0C_ACTUALPARAMETERSNODE_H

#include "Node.h"
#include <vector>

class ExpressionNode;

class ActualParametersNode : Node {

    private:
        std::vector<std::unique_ptr<ExpressionNode>> expressions_;

    public:

        void add_expression(std::unique_ptr<ExpressionNode> expr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_ACTUALPARAMETERSNODE_H
