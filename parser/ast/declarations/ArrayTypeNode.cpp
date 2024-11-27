//
// Created by M on 19.11.2024.
//

#include "ArrayTypeNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"

void ArrayTypeNode::accept(NodeVisitor &visitor) {

}

void ArrayTypeNode::print(ostream &stream) const {
    stream << "ARRAY " << *name_ << " OF " << *type_;
}
