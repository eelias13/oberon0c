//
// Created by M on 22.11.2024.
//

#ifndef OBERON0C_SIMPLEEXPRESSIONNODE_H
#define OBERON0C_SIMPLEEXPRESSIONNODE_H

#include "../Node.h"
#include <vector>
#include <memory>

class TermNode;

enum simpexpr_op {PLUS = 0, MINUS, OR, NONE};

typedef std::pair<simpexpr_op,std::unique_ptr<TermNode>> op_term_pair;

class SimpleExpressionNode : public Node {

    private:

        std::vector<op_term_pair> expressions_;
        static const string op_to_string[4];

    public:

        SimpleExpressionNode(FilePos pos, std::unique_ptr<TermNode> initial_expr, simpexpr_op initial_op = NONE);

        void addTerm(std::unique_ptr<TermNode> term, simpexpr_op op);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_SIMPLEEXPRESSIONNODE_H
