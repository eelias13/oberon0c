//
// Created by M on 15.12.2024.
//

#include "SemanticChecker.h"

SemanticChecker::SemanticChecker(Logger &logger) : logger_(logger) {
    scope_table_ = ScopeTable();
}

// Expressions:
//      --> Use of invalid types must be reported
//      --> Expressions must be type-checked


// Type Checking
//      --> Arithmetic Operators must have INTEGER values and return INTEGERs
//      --> Comparison Operators must have the same type and  return BOOLEANs
//      --> Boolean Operators must have BOOLEAN values and return BOOLEANs
expression_type SemanticChecker::checkType(ExpressionNode& expr) {

    auto type = expr.getNodeType();

    if(type == NodeType::binary_expression){
        auto bin_expr = &dynamic_cast<BinaryExpressionNode&>(expr);

        auto lhs = bin_expr->get_lhs();
        auto rhs = bin_expr->get_rhs();
        auto op = bin_expr->get_op();

        // Comparison Operators
        if(op == Operator::EQ  || op == Operator::NEQ ||
           op == Operator::LEQ || op == Operator::GEQ ||
           op == Operator::LT  || op == Operator::GT){

            if(checkType(*lhs) != checkType(*rhs)){
                logger_.error(expr.pos(), "LHS and RHS of Boolean expression do not have equal types");
                return ERR_TYPE;
            }

            return BOOLEAN;

        }

        // Arithmetic Operators
        else if(op == Operator::PLUS || op == Operator::MINUS ||
                op == Operator::MULT || op == Operator::DIV   ||
                op == Operator::MOD){

            if(checkType(*lhs) != INT){
                logger_.error(expr.pos(), "LHS is not of type INTEGER");
                return ERR_TYPE;
            }
            if(checkType(*rhs) != INT){
                logger_.error(expr.pos(), "RHS is not of type INTEGER");
                return ERR_TYPE;
            }

            return INT;

        }

        // Boolean Operators
        else if(op == Operator::AND || op == Operator::OR){

            if(checkType(*lhs) != BOOLEAN){
                logger_.error(expr.pos(), "LHS is not of type BOOLEAN");
                return ERR_TYPE;
            }
            if(checkType(*rhs) != BOOLEAN){
                logger_.error(expr.pos(), "RHS is not of type BOOLEAN");
                return ERR_TYPE;
            }

            return BOOLEAN;

        }

    }

    else if(type == NodeType::unary_expression){

        auto un_expr = &dynamic_cast<UnaryExpressionNode&>(expr);
        auto op = un_expr->get_op();
        auto inner = un_expr->get_expr();

        if(op == Operator::NEG){
            if(checkType(*inner) != BOOLEAN){
                logger_.error(expr.pos(), "Cannot negate an expression that's not of type BOOLEAN");
                return ERR_TYPE;
            }

            return BOOLEAN;
        }
        else if(op == Operator::MINUS){
            if(checkType(*inner) != INT){
                logger_.error(expr.pos(), "Expression is not of type INTEGER");
                return ERR_TYPE;
            }
            return INT;
        }

    }
    else if(type == NodeType::ident_selector_expression){

        // ? ? ? ? ? ?

    }
    else{
        logger_.error(expr.pos(),"Invalid or empty expression");
        return ERR_TYPE;
    }

    logger_.error(expr.pos(), "Could not deduce expression type");
    return ERR_TYPE;
}

// Theoretically, only Expressions that return an INTEGER can be evaluated
long SemanticChecker::evaluate_expression(ExpressionNode& expr) {

    auto type = expr.getNodeType();

    if(type == NodeType::binary_expression){
        auto bin_expr = &dynamic_cast<BinaryExpressionNode&>(expr);

        auto lhs = bin_expr->get_lhs();
        auto rhs = bin_expr->get_rhs();
        auto op = bin_expr->get_op();

        switch (op) {
            case Operator::MINUS:
                return evaluate_expression(*lhs) - evaluate_expression(*rhs);
            case Operator::MOD:
                return (evaluate_expression(*lhs) % evaluate_expression(*rhs));
            case Operator::DIV:
                return evaluate_expression(*lhs)/ evaluate_expression(*rhs);
            case Operator::MULT:
                return evaluate_expression(*lhs) * evaluate_expression(*rhs);
            case Operator::PLUS:
                return evaluate_expression(*lhs) + evaluate_expression(*rhs);
            default:
                logger_.error(expr.pos(), "Could not evaluate expression to an integer");
                return -1;
        }
    }
    else if(type == NodeType::unary_expression){

        auto un_expr = &dynamic_cast<UnaryExpressionNode&>(expr);
        auto op = un_expr->get_op();
        auto inner = un_expr->get_expr();

        if(op == Operator::MINUS){
            return -evaluate_expression(*inner);
        }

    }

    // Ident Selector Expression?

    logger_.error(expr.pos(), "Could not evaluate expression to an integer");
    return -1;

}

// Module:
//      --> The beginning and ending names should align
//      --> A module opens a new scope
void SemanticChecker::visit(ModuleNode& module) {

    scope_table_.beginScope();
    auto names = module.get_name();

    if(names.first->get_value() != names.second->get_value()){
        logger_.error(module.pos(), "Beginning and ending names of module are unequal: \"" + names.first->get_value() + "\" and \"" + names.second->get_value() + "\"" );
    }

    // visit module.get_declarations

    // visit module.get_statements

    scope_table_.endScope();

}




