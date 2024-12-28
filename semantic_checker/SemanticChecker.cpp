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
// Type is returned as a string with special types denotes by an underscore (as Oberon0 does not allow underscores as the start of identifiers)
string SemanticChecker::checkType(ExpressionNode& expr) {

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
                return "_ERROR";
            }

            return "_BOOLEAN";

        }

        // Arithmetic Operators
        else if(op == Operator::PLUS || op == Operator::MINUS ||
                op == Operator::MULT || op == Operator::DIV   ||
                op == Operator::MOD){

            if(checkType(*lhs) != INT){
                logger_.error(expr.pos(), "LHS is not of type INTEGER");
                return "_ERROR";
            }
            if(checkType(*rhs) != INT){
                logger_.error(expr.pos(), "RHS is not of type INTEGER");
                return "_ERROR";
            }

            return "_INTEGER";

        }

        // Boolean Operators
        else if(op == Operator::AND || op == Operator::OR){

            if(checkType(*lhs) != BOOLEAN){
                logger_.error(expr.pos(), "LHS is not of type BOOLEAN");
                return "_ERROR";
            }
            if(checkType(*rhs) != BOOLEAN){
                logger_.error(expr.pos(), "RHS is not of type BOOLEAN");
                return "_ERROR";
            }

            return "_BOOLEAN";

        }

    }

    else if(type == NodeType::unary_expression){

        auto un_expr = &dynamic_cast<UnaryExpressionNode&>(expr);
        auto op = un_expr->get_op();
        auto inner = un_expr->get_expr();

        if(op == Operator::NEG){
            if(checkType(*inner) != "_BOOLEAN"){
                logger_.error(expr.pos(), "Cannot negate an expression that's not of type BOOLEAN");
                return "_ERROR";
            }

            return "_BOOLEAN";
        }
        else if(op == Operator::MINUS){
            if(checkType(*inner) != "_INTEGER"){
                logger_.error(expr.pos(), "Expression is not of type INTEGER");
                return "_ERROR";
            }
            return "_INTEGER";
        }

    }
    else if(type == NodeType::ident_selector_expression){

        auto id_expr = &dynamic_cast<IdentSelectorExpressionNode&>(expr);
        auto identifier = id_expr->get_identifier();
        auto selector = id_expr->get_selector();

        // check if identifier is defined
        auto identifier_info = scope_table_.lookup(identifier->get_value());
        if(!identifier_info){
            logger_.error(expr.pos(), "Undefined Identifier: " + identifier->get_value());
            return "_ERROR";
        }

        if(!selector || !selector->get_selector()){
            return identifier_info->type;
        }

        // Check the selector chain for validity (visit the selector)

        // Go through the entire selector chain
        auto elements = selector->get_selector();
        for(auto itr = elements->begin(); itr != elements->end(); itr++){

            if(std::get<0>(*itr)){

                // Array Index

            }else{

                // Field Record

            }

        }

    }
    else{
        logger_.error(expr.pos(),"Invalid or empty expression");
        return "_ERROR";
    }

    logger_.error(expr.pos(), "Could not deduce expression type");
    return "_ERROR";
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

// Procedure Declaration:
//      --> Beginning and ending names should align
//      --> A procedure opens a new scope
void SemanticChecker::visit(ProcedureDeclarationNode & procedure) {

    auto names = procedure.get_names();
    if(names.first->get_value() != names.second->get_value()){
        logger_.error(procedure.pos(), "Beginning and ending names of procedure are unequal: \"" + names.first->get_value() + "\" and \"" + names.second->get_value() + "\"" );
    }

    // Check for double declarations
    if(scope_table_.lookup(names.first->get_value(), true)){
        logger_.error(procedure.pos(), "Multiple declarations for procedure '" + names.first->get_value() + "' found. (Note: Oberon0 does not allow overriding functions)");
    }

    // Save the procedure name (before opening up a new scope!)
    scope_table_.insert(names.first->get_value(), Kind::PROCEDURE, &procedure);

    // Open up new scope
    scope_table_.beginScope();

    // Check the parameters

    // Check statements --> visit procedure.get_statements()

    scope_table_.endScope();
}

// Declarations:
//      --> Every identifier should be stored in the scope table with its corresponding kind
void SemanticChecker::visit(DeclarationsNode & declars) {

    auto constants = declars.get_constants();
    auto variables = declars.get_variables();
    auto typenames = declars.get_typenames();
    auto procedures = declars.get_procedures();

    // Constants:
    //      --> (...)
    for(auto itr = constants.begin(); itr != constants.end(); itr++){

        // check for double declarations (only in current scope)
        if(scope_table_.lookup(itr->first,true)){
            logger_.error(declars.pos(),"Multiple Declarations of identifier: " + itr->first);
        }

        // check if expression actually evaluates to a constant

        // insert variable into scope table
        scope_table_.insert(itr->first, Kind::CONSTANT, &declars);

    }

    // Variables:
    //      --> (...)
    for(auto itr = variables.begin(); itr != variables.end(); itr++){

        for(auto el = itr->first.begin(); el != itr->first.end(); el++){

            // check for double declarations (only in current scope)
            if(scope_table_.lookup(*el,true)){
                logger_.error(declars.pos(),"Multiple Declarations of identifier: " + (*el));
            }

            // check for valid types


            // insert variable into symbol table
            scope_table_.insert(*el,Kind::VARIABLE,&declars);

        }
    }

    // Typenames
    //      --> (...)
    for(auto itr = typenames.begin(); itr != typenames.end(); itr++){

        // check for double declarations
        if(scope_table_.lookup(itr->first, true)){
            logger_.error(declars.pos(),"Multiple Declarations of identifier: " + itr->first);
        }

        // check the type definition

        // insert into scope table
        scope_table_.insert(itr->first, Kind::TYPENAME, itr->second);

    }

    // Procedures
    for(auto itr = procedures.begin(); itr != procedures.end(); itr++){
        visit(*(*itr));
    }

}




