//
// Created by M on 22.11.2024.
//

#ifndef OBERON0C_TERMNODE_H
#define OBERON0C_TERMNODE_H

#include "parser/ast/Node.h"
#include <vector>
#include <memory>

class FactorNode;

enum term_ops {MULT = 0, DIV, MOD, AND, TERM_NONE};

typedef std::pair<term_ops, std::unique_ptr<FactorNode>> op_fact_pair;

class TermNode : public Node {

    private:
        std::vector<op_fact_pair> factors_;
        static const string op_to_string[5];

    public:
        TermNode(FilePos pos, std::unique_ptr<FactorNode> first_factor);

        void addFactor(term_ops op, std::unique_ptr<FactorNode> factor);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_TERMNODE_H
