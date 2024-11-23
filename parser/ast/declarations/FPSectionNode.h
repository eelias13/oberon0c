//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_FPSECTIONNODE_H
#define OBERON0C_FPSECTIONNODE_H

#include "IdentListNode.h"
#include "TypeNode.h"

class FPSectionNode : public Node {
    private:
        bool var_included_;
        std::unique_ptr<IdentListNode> param_list_;
        std::unique_ptr<TypeNode> type_;

    public:

        FPSectionNode(FilePos pos, bool var_included,std::unique_ptr<IdentListNode> param_list, std::unique_ptr<TypeNode> type) : Node(NodeType::fp_section,pos), var_included_(var_included), param_list_(std::move(param_list)), type_(std::move(type)) {};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FPSECTIONNODE_H
