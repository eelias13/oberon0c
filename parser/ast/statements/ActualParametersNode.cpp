//
// Created by M on 19.11.2024.
//

#include "ActualParametersNode.h"

void ActualParametersNode::add_expression(std::unique_ptr<ExpressionNode> expr) {
        expressions_.emplace_back(std::move(expr));
}

void ActualParametersNode::accept(NodeVisitor &visitor) {

}

void ActualParametersNode::print(ostream &stream) const {
        stream << "(";

        for(auto itr = expressions_.begin(); itr != expressions_.end(); itr++){
            stream << itr->get();
        }

        stream << ")";
}
