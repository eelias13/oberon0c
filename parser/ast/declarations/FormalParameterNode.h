//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_FORMALPARAMETERNODE_H
#define OBERON0C_FORMALPARAMETERNODE_H

#include "parser/ast/Node.h"
#include <vector>
#include <memory>

class IdentNode;
class TypeNode;

typedef std::tuple<bool,std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>>,std::unique_ptr<TypeNode>> fp_section;

class FormalParameterNode : public Node {

    private:

        std::vector<std::unique_ptr<fp_section>> parameter_sections_;
    public:

        explicit FormalParameterNode(FilePos pos);

        void add_parameter_section(std::unique_ptr<fp_section> section);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FORMALPARAMETERNODE_H
