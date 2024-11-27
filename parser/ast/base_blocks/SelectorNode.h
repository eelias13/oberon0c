//
// Created by M on 21.11.2024.
//

#ifndef OBERON0C_SELECTORNODE_H
#define OBERON0C_SELECTORNODE_H

#include "parser/ast/base_blocks/IdentNode.h"
#include <vector>
#include <memory>

class ExpressionNode;

typedef std::tuple<bool,std::unique_ptr<IdentNode>,std::unique_ptr<ExpressionNode>> id_indx_tuple;

class SelectorNode : public Node {

    private:
        std::vector<id_indx_tuple> selectors;

    public:
        explicit SelectorNode(FilePos pos);

        void add_field(std::unique_ptr<IdentNode> ident);
        void add_index(std::unique_ptr<ExpressionNode> expr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_SELECTORNODE_H
