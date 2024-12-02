//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_FPSECTIONNODE_H
#define OBERON0C_FPSECTIONNODE_H

#include "parser/ast/Node.h"
#include <vector>

class IdentNode;
class TypeNode;

class FPSectionNode : public Node {
    private:
        bool var_included_;
        std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> param_list_;
        std::unique_ptr<TypeNode> type_;

    public:

        FPSectionNode(FilePos pos, bool var_included,std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> param_list, std::unique_ptr<TypeNode> type);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FPSECTIONNODE_H
