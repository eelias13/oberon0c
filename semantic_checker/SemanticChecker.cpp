//
// Created by M on 15.12.2024.
//

#include "SemanticChecker.h"
#include <limits>

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

            auto l_type = checkType(*lhs);
            auto r_type = checkType(*rhs);

            if(trace_type(l_type).starts_with("_RECORD") ||
               trace_type(r_type).starts_with("_RECORD") ||
               trace_type(l_type).starts_with("_ARRAY") ||
               trace_type(r_type).starts_with(("_ARRAY"))){
                logger_.error(expr.pos(), "Illegal use of comparison operators with array/record types.");
                return "_ERROR";
            }
            if(l_type != r_type){
                logger_.error(expr.pos(), "LHS and RHS of Boolean expression do not have equal types.");
                return "_ERROR";
            }

            return "BOOLEAN";

        }

        // Arithmetic Operators
        else if(op == Operator::PLUS || op == Operator::MINUS ||
                op == Operator::MULT || op == Operator::DIV   ||
                op == Operator::MOD){

            if(trace_type(checkType(*lhs)) != "INTEGER"){
                logger_.error(expr.pos(), "LHS of arithmetic expression is not of type INTEGER.");
                return "_ERROR";
            }
            if(trace_type(checkType(*rhs)) != "INTEGER"){
                logger_.error(expr.pos(), "RHS of arithmetic expression is not of type INTEGER.");
                return "_ERROR";
            }

            return "INTEGER";

        }

        // Boolean Operators
        else if(op == Operator::AND || op == Operator::OR){

            if(checkType(*lhs) != "BOOLEAN"){
                logger_.error(expr.pos(), "LHS of Boolean expression is not of type BOOLEAN.");
                return "_ERROR";
            }
            if(checkType(*rhs) != "BOOLEAN"){
                logger_.error(expr.pos(), "RHS of Boolean expression is not of type BOOLEAN.");
                return "_ERROR";
            }

            return "BOOLEAN";

        }

    }

    else if(type == NodeType::unary_expression){

        auto un_expr = &dynamic_cast<UnaryExpressionNode&>(expr);
        auto op = un_expr->get_op();
        auto inner = un_expr->get_expr();

        if(op == Operator::NOT){
            if(checkType(*inner) != "BOOLEAN"){
                logger_.error(expr.pos(), "Cannot negate an expression that's not of type BOOLEAN.");
                return "_ERROR";
            }

            return "BOOLEAN";
        }
        else if(op == Operator::NEG){
            if(trace_type(checkType(*inner)) != "INTEGER"){
                logger_.error(expr.pos(), "Expression is not of type INTEGER.");
                return "_ERROR";
            }
            return "INTEGER";
        }
        else if(op == Operator::NO_OPERATOR || op == Operator::PAREN){
            return checkType(*inner);
        }

    }
    else if(type == NodeType::ident_selector_expression){

        auto id_expr = &dynamic_cast<IdentSelectorExpressionNode&>(expr);
        return checkSelectorType(*id_expr);

    }
    else if(type == NodeType::integer){
        return "INTEGER";
    }
    else{
        logger_.error(expr.pos(),"Invalid or empty expression.");
        return "_ERROR";
    }

    logger_.error(expr.pos(), "Could not deduce expression type.");
    return "_ERROR";
}

// Checks if expression is constant and if so evaluates it (returns quiet_NaN() in cases where errors occur)
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
                logger_.error(expr.pos(), "Could not evaluate expression to an integer.");
                return std::numeric_limits<long>::quiet_NaN();
        }
    }
    else if(type == NodeType::unary_expression){

        auto un_expr = &dynamic_cast<UnaryExpressionNode&>(expr);
        auto op = un_expr->get_op();
        auto inner = un_expr->get_expr();

        if(op == Operator::NEG){
            return -evaluate_expression(*inner);
        }
        else if(op == Operator::NO_OPERATOR || op == Operator::PAREN){
            return evaluate_expression(*inner);
        }

    }
    else if(type == NodeType::ident_selector_expression){

        // Can only be evaluated if the expression consists of only a constant identifier
        auto id_sel_expr = &dynamic_cast<IdentSelectorExpressionNode&>(expr);

        if(id_sel_expr->get_selector() && !id_sel_expr->get_selector()->get_selector()){
            logger_.error(expr.pos(), "Constant expression contains non-constant elements (array-indexing or record-fields).");
            return std::numeric_limits<long>::quiet_NaN();
        }

        auto id_info = scope_table_.lookup(id_sel_expr->get_identifier()->get_value());
        if(id_info->kind != Kind::CONSTANT){
            logger_.error(expr.pos(), "Constant expression contains non-constant identifiers.");
            return std::numeric_limits<long>::quiet_NaN();
        }

        // Get value of constant
        if(!id_info->node){
            logger_.error(expr.pos(), "Could not find value of the constant: '" + id_sel_expr->get_identifier()->get_value() + "'.");
            return std::numeric_limits<long>::quiet_NaN();
        }

        auto constant_expr = dynamic_cast<const ExpressionNode*>(id_info->node);
        return evaluate_expression(const_cast<ExpressionNode &>(*constant_expr));

    }
    else if(type == NodeType::integer){
        auto integer_node = &dynamic_cast<IntNode&>(expr);
        return integer_node->get_value();
    }

    logger_.error(expr.pos(), "Could not evaluate expression to an integer.");
    return std::numeric_limits<long>::quiet_NaN();

}

// Type inference for an ident-selector expression
string SemanticChecker::checkSelectorType(IdentSelectorExpressionNode& id_expr) {
    auto identifier = id_expr.get_identifier();
    auto selector = id_expr.get_selector();

    // check if identifier is defined
    auto identifier_info = scope_table_.lookup(identifier->get_value());
    if(!identifier_info){
        logger_.error(id_expr.pos(), "Unknown Identifier: " + identifier->get_value());
        return "_ERROR";
    }

    if(!selector || !selector->get_selector()){
        return identifier_info->type;
    }

    // Check the selector chain for validity (visit the selector)
    return checkSelectorChain(*identifier, *selector);
}

// Traces back types to handle type-aliases like the following:
// TYPE INTARRAY = ARRAY 20 OF INTEGER
// TYPE INA      = INTARRAY
// TYPE ABC      = INA
string SemanticChecker::trace_type(const string& initial_type) {

    string current_alias = initial_type;
    IdentInfo* curr_info = scope_table_.lookup(initial_type);
    while(curr_info && (current_alias != curr_info->type)){
        current_alias = curr_info->type;
        curr_info = scope_table_.lookup(current_alias);
    }

    return current_alias;

}

// Selector:
//      --> Validity has to be checked through the entire chain
//      --> For Array-Indexing:
//                  * Indexed object must actually have an array-type
//                  * Index must have the type INTEGER
//      --> For Field-Selection:
//                  * Object must actually have a record type
//                  * Identifier must refer to an actual field of that record type
string SemanticChecker::checkSelectorChain(IdentNode& ident, SelectorNode& selector) {
    IdentInfo* prev_info = scope_table_.lookup(ident.get_value());

    if(!selector.get_selector()){
        return prev_info->type;
    }

    auto chain = selector.get_selector();

    // Traverse entire chain
    // Note that the validity of the first selector cannot be checked inside this function
    for(auto itr = chain->begin(); itr != chain->end(); itr++){

        if(std::get<0>(*itr)){

            // Array Index

            // Object must be an array type
            if(!trace_type(prev_info->type).starts_with("_ARRAY")){
                logger_.error(selector.pos(), "Tried to index non-array object.");
                return "_ERROR";
            }

            // Index must evaluate to a non-negative integer
            auto expr = std::get<2>(*itr).get();
            if(trace_type(checkType(*expr)) != "INTEGER"){  // && !isConstant(*expr)
                logger_.error(selector.pos(), "Array index does not evaluate to an integer.");
                return "_ERROR";
            }

            // Update prev_info
            // Recall Array Types have the following string form: "ARRAY_<DIM>_OF_<TYPE>"
            string arr_string = trace_type(prev_info->type).erase(0,7);  // Cuts off "ARRAY_"
            arr_string = arr_string.erase(0,arr_string.find('_') + 1);            // Cuts off "<DIM>_
            arr_string = arr_string.erase(0,arr_string.find('_') + 1);            // Cuts off "OF_"

            prev_info = scope_table_.lookup(arr_string);

            if(!prev_info){
                logger_.error(selector.pos(), "Unable to get information for type '" + arr_string + "'");
                return "_ERROR";
            }


        }else{

            // Record Field

            // Object must actually have a record type
            if(!trace_type(prev_info->type).starts_with("_RECORD")){
                logger_.error(selector.pos(), "Tried to access field of a non-record object.");
                return "_ERROR";
            }

            // Identifier must refer to an actual field of that record type
            string record_name = scope_table_.lookup(prev_info->type)->name;
            string field_type = scope_table_.lookup_field(record_name,std::get<1>(*itr)->get_value());
            if(field_type == "_ERROR"){
                logger_.error(selector.pos(), "Tried to access invalid field of record type '" + record_name + "' (Field: " + std::get<1>(*itr)->get_value() + ").");
                return "_ERROR";
            }

            // Update prev_info
            prev_info = scope_table_.lookup(field_type);

            if(!prev_info){
                logger_.error(selector.pos(), "Unable to get information for type '" + field_type + "'");
                return "_ERROR";
            }

        }

    }

    return prev_info->type;
}

// Returns a string describing the type of given TypeNode ("_ARRAY_<DIM>_OF_<TYPE_" for arrays, "_RECORD" for records)
string SemanticChecker::get_type_string(TypeNode &type) {

    if(type.getNodeType() == NodeType::ident){
        return dynamic_cast<IdentNode&>(type).get_value();
    }
    else if(type.getNodeType() == NodeType::array_type){
        auto array_node = &dynamic_cast<ArrayTypeNode&>(type);
        long dim = evaluate_expression(*array_node->get_dimensions());
        return "_ARRAY_" + to_string(dim) + "_OF_" + get_type_string(*array_node->get_type());
    }
    else if(type.getNodeType() == NodeType::record_type){
        return "_RECORD";
    }

    std::cerr << "Invalid NodeType passed as TypeNode!" << std::endl;
    return "";

}

// Module:
//      --> The beginning and ending names should align
//      --> A module opens a new scope
void SemanticChecker::visit(ModuleNode& module) {

    scope_table_.beginScope();

    // Insert pre-defined types "INTEGER" and "BOOLEAN"
    scope_table_.insert("INTEGER",Kind::TYPENAME, nullptr, "INTEGER");
    scope_table_.insert("BOOLEAN",Kind::TYPENAME, nullptr, "BOOLEAN");

    auto names = module.get_name();

    if(names.first->get_value() != names.second->get_value()){
        logger_.error(module.pos(), "Beginning and ending names of module are unequal: \"" + names.first->get_value() + "\" and \"" + names.second->get_value() + "\"." );
    }

    // validate declarations
    visit(*module.get_declarations());

    // validate statements
    visit(*module.get_statements());

    scope_table_.endScope();
}

// Procedure Declaration:
//      --> Beginning and ending names should align
//      --> A procedure opens a new scope
void SemanticChecker::visit(ProcedureDeclarationNode & procedure) {

    auto names = procedure.get_names();
    if(names.first->get_value() != names.second->get_value()){
        logger_.error(procedure.pos(), "Beginning and ending names of procedure are unequal: \"" + names.first->get_value() + "\" and \"" + names.second->get_value() + "\"." );
    }

    // Check for double declarations
    if(scope_table_.lookup(names.first->get_value(), true)){
        logger_.error(procedure.pos(), "Multiple declarations for procedure '" + names.first->get_value() + "' found (Note: Oberon0 does not allow overriding functions).");
    }

    // Save the procedure name (before opening up a new scope!)
    scope_table_.insert(names.first->get_value(), Kind::PROCEDURE, &procedure);

    // Open up new scope
    scope_table_.beginScope();

    // Check the parameters
    auto params = procedure.get_parameters();

    if(params){
        for(auto itr = params->begin(); itr != params->end(); itr++){

            bool is_var = std::get<0>(**itr);

            // Check Type definition
            TypeNode* type = std::get<2>(**itr).get();
            if(type->getNodeType() == NodeType::array_type || type->getNodeType() == NodeType::record_type){
                logger_.warning(type->pos(), "New Type defined in formal parameters of function. Actual Parameter will never be able to fulfill this type (Note: The Oberon0 compiler follows name-equivalence, not structural equivalence).");
            }
            visit(*type);

            for(auto var = std::get<1>(**itr)->begin(); var != std::get<1>(**itr)->end(); var++){

                // Check for double definitions
                if(scope_table_.lookup(var->get()->get_value(), true)){
                    logger_.error(var->get()->pos(), "Multiple use of the same parameter name.");
                }

                scope_table_.insert(var->get()->get_value(), Kind::VARIABLE, var->get(), get_type_string(*type));

            }

        }

    }

    // Check declarations
    visit(*procedure.get_declarations());

    // Check statements
    visit(*procedure.get_statements());

    scope_table_.endScope();
}

// Declarations:
//      --> Every identifier should be stored in the scope table with its corresponding information
//      --> Erroneous declarations/definitions should be reported
void SemanticChecker::visit(DeclarationsNode & declars) {

    auto constants = declars.get_constants();
    auto variables = declars.get_variables();
    auto typenames = declars.get_typenames();
    auto procedures = declars.get_procedures();

    // Constants:
    //      --> Expression must evaluate to a constant
    for(auto itr = constants.begin(); itr != constants.end(); itr++){

        // check for double declarations (only in current scope)
        if(scope_table_.lookup(itr->first,true)){
            logger_.error(declars.pos(),"Multiple declarations of identifier: " + itr->first);
        }

        // check if expression actually evaluates to a constant
        if(std::isnan(static_cast<double>(evaluate_expression(*itr->second)))){
            logger_.error(itr->second->pos(), "Right hand side of constant does not evaluate to a constant.");
        }

        // insert variable into scope table
        scope_table_.insert(itr->first, Kind::CONSTANT, itr->second, checkType(*itr->second));

    }

    // Typenames
    //      --> Type definition must be valid
    for(auto itr = typenames.begin(); itr != typenames.end(); itr++){

        // check for double declarations
        if(scope_table_.lookup(itr->first, true)){
            logger_.error(declars.pos(),"Multiple Declarations of identifier: " + itr->first);
        }

        // check the type definition
        visit(*itr->second);

        // insert into scope table
        scope_table_.insert(itr->first, Kind::TYPENAME, itr->second, get_type_string(*itr->second));

        // if the type referred to a record-type, then we also store the record information in the scope table
        if(itr->second->getNodeType() == NodeType::record_type){
            scope_table_.insert_record(itr->first, key_value_map(dynamic_cast<RecordTypeNode&>(*itr->second)));
        }

    }

    // Variables:
    //      --> Type must be valid
    for(auto itr = variables.begin(); itr != variables.end(); itr++){

        // check for valid types (i.e., visit Type Node)
        visit(*itr->second);

        for(auto el = itr->first.begin(); el != itr->first.end(); el++){

            // check for double declarations (only in current scope)
            if(scope_table_.lookup(*el,true)){
                logger_.error(declars.pos(),"Multiple Declarations of identifier: " + (*el));
            }


            // insert variable into symbol table
            scope_table_.insert(*el,Kind::VARIABLE,itr->second, get_type_string(*itr->second));

        }
    }

    // Procedures
    for(auto itr = procedures.begin(); itr != procedures.end(); itr++){
        visit(*(*itr));
    }

}

// Type: (Recall that Type is an abstract class)
void SemanticChecker::visit(TypeNode &node) {
    if(node.getNodeType() == NodeType::ident){
        visit(dynamic_cast<IdentNode&>(node));
    }
    else if(node.getNodeType() == NodeType::array_type){
        visit(dynamic_cast<ArrayTypeNode&>(node));
    }
    else if(node.getNodeType() == NodeType::record_type){
        visit(dynamic_cast<RecordTypeNode&>(node));
    }
}

// Identifier:
//      --> Must refer to a valid type (Note that this visit method is only called on typenames)
void SemanticChecker::visit(IdentNode &node) {
    auto info = scope_table_.lookup(node.get_value());
    if(!info){
        logger_.error(node.pos(), "Use of unknown identifier: '" + node.get_value() + "'." );
        return;
    }

    if(info->kind != Kind::TYPENAME){
        logger_.error(node.pos(), "Identifier '" + node.get_value() + "' does not refer to a type.");
    }

}

// ArrayType:
//      --> Specified dimension must evaluate to an integer greater than zero
//      --> Specified type must exist (and be a type)
void SemanticChecker::visit(ArrayTypeNode &node) {

    auto dim_expr = node.get_dimensions();
    long dim = evaluate_expression(*dim_expr);

    // Check dimensions
    if(std::isnan(static_cast<double>(dim))){
        logger_.error(dim_expr->pos(), "Specified array dimensions do not evaluate to a constant.");
    }
    else if(dim <= 0){
        logger_.error(dim_expr->pos(), "Cannot create array of size " + to_string(dim) + ".");
    }

    // Check type
    auto type = node.get_type();
    visit(*type);

}

// RecordType:
//      --> RecordType definition opens a new scope
//      --> All field names must be unique
//      --> Types of the fields must be valid
//      --> RecordType is inserted correctly in ScopeTable
void SemanticChecker::visit(RecordTypeNode& node) {

    scope_table_.beginScope();

    auto fields = node.get_fields();

    for(auto field_itr = fields.begin(); field_itr != fields.end(); field_itr++){

        // Check for double names
        for(auto field_name = field_itr->first.begin(); field_name != field_itr->first.end(); field_name++){
            if(scope_table_.lookup(*field_name, true)){
                logger_.error(node.pos(), "Multiple definitions of record field '" + *field_name + "'.");
            }

            // Note: Since this scope is only used to check for double definitions and immediately closed after that,
            //       It doesn't *really* matter what we put into the scope table for the fields
            scope_table_.insert(*field_name, Kind::VARIABLE, nullptr);

        }

        // Check for valid type definition
        visit(*field_itr->second);

    }

    scope_table_.endScope();

}

// Fills a key-value-map-vector which is needed to place record types into the scope table
std::vector<std::pair<string, string>> SemanticChecker::key_value_map(RecordTypeNode &node) {
    std::vector<std::pair<string,string>> key_value_map;

    auto fields = node.get_fields();
    for(auto field_itr = fields.begin(); field_itr != fields.end(); field_itr++){
        for(auto field_name = field_itr->first.begin(); field_name != field_itr->first.end(); field_name++){
            key_value_map.emplace_back(*field_name, get_type_string(*field_itr->second));
        }
    }

    return key_value_map;
}

// StatementSequence:
//      --> Each statement has to be valid
void SemanticChecker::visit(StatementSequenceNode &node) {
    auto statements = node.get_statements();
    for(auto itr = statements->begin(); itr != statements->end(); itr++){
        visit(**itr);
    }
}

// Statement: (Recall that Statement is an abstract class
void SemanticChecker::visit(StatementNode& node){
    if(node.getNodeType() == NodeType::assignment){
        visit(dynamic_cast<AssignmentNode&>(node));
    }
    else if(node.getNodeType() == NodeType::if_statement){
        visit(dynamic_cast<IfStatementNode&>(node));
    }
    else if(node.getNodeType() == NodeType::while_statement){
        visit(dynamic_cast<WhileStatementNode&>(node));
    }
    else if(node.getNodeType() == NodeType::repeat_statement){
        visit(dynamic_cast<RepeatStatementNode&>(node));
    }
    else if(node.getNodeType() == NodeType::procedure_call){
        visit(dynamic_cast<ProcedureCallNode&>(node));
    }
}

// Assignment:
//      --> LHS must be a non-const variable
//      --> RHS must have the same type as the LHS
void SemanticChecker::visit(AssignmentNode& node) {

    // Check LHS
    auto lhs_id = node.get_variable()->get_value();
    auto lhs_id_info = scope_table_.lookup(lhs_id);

    if(!lhs_id_info){
        logger_.error(node.pos(), "Use on unknown identifier: '" + lhs_id + "'." );
        return;
    }

    if(lhs_id_info->kind == Kind::CONSTANT){
        logger_.error(node.pos(), "Tried to re-assign a value to the constant '" + lhs_id + "'.");
        return;
    }

    if(lhs_id_info->kind != Kind::VARIABLE){
        logger_.error(node.pos(), "Can't assign a value to a non-variable.");
        return;
    }

    // Check Selector / Get Type of Variable
    string lhs_type = lhs_id_info->type;
    if(node.get_selector()) {
        lhs_type = checkSelectorChain(*node.get_variable(), *node.get_selector());
        if (lhs_type == "_ERROR") {
            return;
        }
    }

    // Check RHS
    auto rhs = node.get_expr();
    string expr_type = checkType(*rhs);

    if(expr_type != lhs_type && expr_type != "_ERROR"){ // We exclude the error case to avoid too many exceptions
        logger_.error(node.pos(), "Cannot assign something of type '" + expr_type + "' to a variable of type '" + lhs_type + "'.");
        return;
    }

    // If both have a record type, there needs to be more checking done

}

// If Statements:
//      --> All Expressions must evaluate to type BOOLEAN
//      --> All Statements must be valid
void SemanticChecker::visit(IfStatementNode &node) {

    // Initial If-Then
    auto condition = node.get_condition();
    if(checkType(*condition) != "BOOLEAN"){
        logger_.error(condition->pos(), "Condition of If-Statement does not evaluate to a BOOLEAN.");
    }
    visit(*node.get_then());

    // Else-Ifs
    auto else_ifs = node.get_else_ifs();
    for(auto itr = else_ifs->begin(); itr != else_ifs->end(); itr++){

        if(checkType(*itr->first) != "BOOLEAN"){
            logger_.error(itr->first->pos(), "Condition of Else-If-Statement does not evaluate to a BOOLEAN.");
        }
        visit(*itr->second);
    }

    // Else
    auto else_statement = node.get_else();
    if(else_statement){
        visit(*else_statement);
    }

}

// Repeat Statement:
//      --> Condition must evaluate to a Boolean
//      --> Statements must be valid
void SemanticChecker::visit(RepeatStatementNode &node) {

    auto condition = node.get_expr();
    if(checkType(*condition) != "BOOLEAN"){
        logger_.error(node.pos(), "Condition of Repeat-Statement does not evaluate to the type BOOLEAN.");
    }

    // Validate Statements
    visit(*node.get_statements());
}

// While Statement:
//      --> Condition must evaluate to a Boolean
//      --> Statements must be valid
void SemanticChecker::visit(WhileStatementNode& node) {
    auto condition = node.get_expr();
    if(checkType(*condition) != "BOOLEAN"){
        logger_.error(node.pos(), "Condition of While-Loop does not evaluate to the type BOOLEAN.");
    }

    // Validate Statements
    visit(*node.get_statements());
}

// ProcedureCall:
//      --> Identifier must refer to a procedure
//      --> In Oberon0, the selector cannot exist
//      --> Number of actual parameters must equal to the number of formal parameters
//      --> The types of the actual parameters must equal the types of the formal parameters
void SemanticChecker::visit(ProcedureCallNode& node) {

    // Check Name
    auto ident = node.get_name();
    auto ident_info = scope_table_.lookup(ident->get_value());

    if(!ident_info){
        logger_.error(ident->pos(), "Call of unknown procedure: '" + ident->get_value() + "'.");
        return;
    }

    if(ident_info->kind != Kind::PROCEDURE){
        logger_.error(ident->pos(), "'" + ident->get_value() + "' does not refer to a procedure.");
        return;
    }

    // Check Selector
    auto selector = node.get_selector();
    if(selector && !selector->get_selector()->empty()){
        logger_.error(selector->pos(), "Call to array-index or record-field cannot refer to a procedure (In Oberon0).");
        return;
    }

    // Get Function declaration
    auto *procedure_decl = dynamic_cast<const ProcedureDeclarationNode*>(ident_info->node);
    int formal_parameter_nr = procedure_decl->get_parameter_number();
    auto actual_parameters = node.get_parameters();

    if(!actual_parameters){
        if(formal_parameter_nr == 0){
            return;
        }
        else{
            logger_.error(node.pos(), "No parameters given for call to procedure '" + ident->get_value() + "'");
            return;
        }
    }

    if(actual_parameters->size() != formal_parameter_nr){
        logger_.error(node.pos(), "Number of actual parameters does not match the definition of '" + ident->get_value() + "' (Expected: " + to_string(formal_parameter_nr) + ", got: " + to_string(actual_parameters->size()) + ")." );
        return;
    }

    // Type Checking Actual parameters
    auto formal_parameters = procedure_decl->get_parameters();
    auto act_param_itr = actual_parameters->begin();

    if(formal_parameters){
        for(auto fp_section_itr = formal_parameters->begin(); fp_section_itr != formal_parameters->end(); fp_section_itr++){

            auto curr_type = std::get<2>(**fp_section_itr).get();
            string curr_type_str = get_type_string(*curr_type);

            for(auto formal_param = std::get<1>(**fp_section_itr)->begin(); formal_param != std::get<1>(**fp_section_itr)->end() ; formal_param++){

                if(curr_type_str.starts_with("_RECORD")){
                    logger_.error(node.pos(), "Cannot correctly check type of given parameter for procedure '" + ident->get_value() + "' since record types can be compared by name only.");
                }
                else{
                    string act_param_type = checkType(**act_param_itr);
                    if(act_param_type != curr_type_str){
                        logger_.error(node.pos(), "Type of actual parameter does not match type of formal parameter (expected '" + curr_type_str + "', got '" + act_param_type + "').");
                    }
                }

                act_param_itr++;

            }

        }
    }

}


// Left empty, but needed to implement NodeVisitor
void SemanticChecker::visit(IntNode &node) {(void)node;}
void SemanticChecker::visit(UnaryExpressionNode &node) {(void)node;}
void SemanticChecker::visit(BinaryExpressionNode &node) {(void)node;}
void SemanticChecker::visit(IdentSelectorExpressionNode &node) {(void)node;}
void SemanticChecker::visit(SelectorNode &node) {(void)node;}

void SemanticChecker::validate_program(ModuleNode &node) {
    visit(node);
}


