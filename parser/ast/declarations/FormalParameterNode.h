//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_FORMALPARAMETERNODE_H
#define OBERON0C_FORMALPARAMETERNODE_H

#include <vector>
#include "parser/ast/Node.h"
#include <memory>

class FPSectionNode;

class FormalParameterNode : public Node {

    private:

        std::vector<std::unique_ptr<FPSectionNode>> parameter_sections_;
    public:

        explicit FormalParameterNode(FilePos pos);

        void add_parameter_section(std::unique_ptr<FPSectionNode> section);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FORMALPARAMETERNODE_H
