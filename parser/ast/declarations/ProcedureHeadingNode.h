//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREHEADINGNODE_H
#define OBERON0C_PROCEDUREHEADINGNODE_H

#include "parser/ast/Node.h"
#include <vector>

class IdentNode;
class TypeNode;

typedef std::tuple<bool,std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>>,std::unique_ptr<TypeNode>> fp_section;

typedef std::vector<std::unique_ptr<fp_section>> formal_parameters;


class ProcedureHeadingNode : public Node {

    private:
        std::unique_ptr<IdentNode> name_;
        std::unique_ptr<formal_parameters> params_;

    public:

        ProcedureHeadingNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<formal_parameters> params = nullptr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_PROCEDUREHEADINGNODE_H
