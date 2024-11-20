//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_FORMALPARAMETERNODE_H
#define OBERON0C_FORMALPARAMETERNODE_H

#include "FPSectionNode.h"

class FormalParameterNode : Node {

    public:

        std::vector<std::unique_ptr<FPSectionNode>> parameter_sections_;
    private:

        void add_parameter_section(std::unique_ptr<FPSectionNode> section){parameter_sections_.emplace_back(std::move(section));};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FORMALPARAMETERNODE_H
