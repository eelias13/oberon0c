//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_PROCEDURECALLNODE_H
#define OBERON0C_PROCEDURECALLNODE_H

#include "ActualParametersNode.h"

class IdentNode;
class SelectorNode;

class ProcedureCallNode : Node {

    private:
        std::unique_ptr<IdentNode> name_;
        std::unique_ptr<SelectorNode> selector_;

        std::unique_ptr<ActualParametersNode> parameters_;

    public:

        explicit ProcedureCallNode(const NodeType nodeType, FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<SelectorNode> selector, std::unique_ptr<ActualParametersNode> parameters) : Node(nodeType,pos), name_(name), selector_(selector), parameters_(parameters) { };

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;


};


#endif //OBERON0C_PROCEDURECALLNODE_H
