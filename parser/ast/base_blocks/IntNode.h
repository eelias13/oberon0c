//
// Created by M on 21.11.2024.
//

#ifndef OBERON0C_INTNODE_H
#define OBERON0C_INTNODE_H

#include "parser/ast/arithmetic/FactorNode.h"

class IntNode : public FactorNode {

    private:
        long value_;

    public:

        IntNode(FilePos pos, long value) : FactorNode(NodeType::integer,pos), value_(value){};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_INTNODE_H
