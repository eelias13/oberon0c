// Implementation of the Parser used by the Oberon0 Compiler
//
// Created by Max Galetskiy on 16.11.2024.
//

#include "Parser.h"
#include "../scanner/IdentToken.h"

// ident -> letter (letter | digit)*  (already recognized by the scanner in full)
std::unique_ptr<IdentNode> Parser::ident() {
    logger_.info("Ident");
    auto start = scanner_.peek()->start();
    auto token = scanner_.peek();
    if(token->type() == TokenType::const_ident){
        auto ident_pntr = scanner_.next();
        auto ident_token = dynamic_cast<const IdentToken*>(ident_pntr.get());
        return std::make_unique<IdentNode>(start, ident_token->value());                                    // success
    }

    return nullptr; // Syntax Error (Expected Identifier, but got)

}

// integer -> digit (digit)*  (already recognized by the scanner in full)
std::unique_ptr<IntNode> Parser::integer() {
    logger_.info("Integer");
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::int_literal || token_type == TokenType::short_literal || token_type == TokenType::long_literal){
        auto int_pntr = scanner_.next();

        long int_value;
        switch(token_type){
            case TokenType::int_literal:
                int_value = dynamic_cast<const IntLiteralToken*>(int_pntr.get())->value();
                break;
            case TokenType::short_literal:
                int_value = dynamic_cast<const ShortLiteralToken*>(int_pntr.get())->value();
                break;
            case TokenType::long_literal:
                int_value = dynamic_cast<const LongLiteralToken*>(int_pntr.get())->value();
                break;
        }


        return std::make_unique<IntNode>(int_pntr->start(),int_value); // Success
    }

    return nullptr; // Syntax Error (Expected Integer, but got)
}

// number -> integer
std::unique_ptr<IntNode> Parser::number() {
    return integer();
}

// selector -> ("." ident | "[" expression "]" )*
std::unique_ptr<SelectorNode> Parser::selector() {
    logger_.info("Selector");
    auto start = scanner_.peek()->start();
    auto token = scanner_.peek();

    auto selector = std::make_unique<SelectorNode>(start);

    while(token->type() == TokenType::period || token->type() == TokenType::lbrack){

        if(token->type() == TokenType::period){
            scanner_.next();
            auto identifier = ident();
            selector->add_field(std::move(identifier));
        }
        else{
            scanner_.next();
            auto expr = expression();
            token = scanner_.peek();

            if(token->type() == TokenType::rbrack){
                scanner_.next();
                selector->add_index(std::move(expr));
            }else{
                return nullptr; // Syntax Error (Expected ")" but got)
            }
        }

        token = scanner_.peek();

    }

    return selector;

}

// factor -> ident selector | number | "(" expression ")" | "~" factor
std::unique_ptr<FactorNode> Parser::factor() {
    logger_.info("Factor");
    auto start = scanner_.peek()->start();
    auto token = scanner_.peek();

    if(token->type() == TokenType::const_ident){
        auto id = ident();
        auto sel = selector();
        return std::make_unique<IdentSelectorFactorNode>(start,std::move(id),std::move(sel));
    }

    else if(token->type() == TokenType::int_literal || token->type() == TokenType::short_literal || token->type() == TokenType::long_literal){
        return number();
    }

    else if(token->type() == TokenType::lparen){
        scanner_.next();
        auto expr = expression();
        token = scanner_.peek();
        if(token->type() == TokenType::rparen){
            scanner_.next();
            return std::make_unique<ExpressionInFactorNode>(start,std::move(expr));
        }
        return nullptr;
    }

    else if(token->type() == TokenType::op_not){
        auto factor_token = scanner_.next();
        auto fact = factor();
        return std::make_unique<NegatedFactorNode>(factor_token->start(),std::move(fact));
    }

    else{
        // syntax error (Expected identifier, integer, "(" or "~", but got)
        return nullptr;
    }

}

// term -> factor (("*" | "DIV" | "MOD" | "&") factor)*
std::unique_ptr<TermNode>  Parser::term() {
    logger_.info("Term");
    auto start = scanner_.peek()->start();
    auto fact = factor();
    auto token_type = scanner_.peek()->type();

    auto term_node = std::make_unique<TermNode>(start,std::move(fact));

    while(token_type == TokenType::op_times || token_type == TokenType::op_div ||
          token_type == TokenType::op_mod   || token_type == TokenType::op_and) {

        term_ops op;
        switch (token_type) {
            case TokenType::op_times:
                op = term_ops::MULT;
                break;
            case TokenType::op_div:
                op = term_ops::DIV;
                break;
            case TokenType::op_mod:
                op = term_ops::MOD;
                break;
            case TokenType::op_and:
                op = term_ops::AND;
                break;
        }

        scanner_.next();
        term_node->addFactor(op,factor());
        token_type = scanner_.peek()->type();
    }

    return term_node;

}

// SimpleExpression -> ("+" | "-" | €) term (("+"|"-"|"OR") term)*
std::unique_ptr<SimpleExpressionNode> Parser::simple_expression() {

    logger_.info("Simple Expression");

    auto start = scanner_.peek()->start();
    auto token = scanner_.peek();
    auto op = simpexpr_op::NONE;

    if(token->type() == TokenType::op_plus || token->type() == TokenType::op_minus){
        op = (token->type() == TokenType::op_plus) ? simpexpr_op::PLUS : simpexpr_op::OR;
        scanner_.next();
    }
    auto fst_term = term();

    auto simp_expr = std::make_unique<SimpleExpressionNode>(start,std::move(fst_term),op);

    token = scanner_.peek();
    while(token->type() == TokenType::op_plus || token->type() == TokenType::op_minus || token->type() == TokenType::op_or){

        simpexpr_op op;
        switch (token->type()) {
            case TokenType::op_plus:
                op = simpexpr_op::PLUS;
                break;
            case TokenType::op_minus:
                op = simpexpr_op::MINUS;
                break;
            case TokenType::op_or:
                op = simpexpr_op::OR;
                break;
        }

        scanner_.next();
        auto trm = term();
        simp_expr->addTerm(std::move(trm),op);

        token = scanner_.peek();
    }

    return simp_expr;

}

// Expression -> SimpleExpression (("="|"#"|"<"|"<="|">"|">=") SimpleExpression)?
std::unique_ptr<ExpressionNode> Parser::expression() {
    logger_.info("Expression");
    auto start = scanner_.peek()->start();
    auto fst_expr = simple_expression();

    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::op_eq || token_type == TokenType::op_neq ||
       token_type == TokenType::op_lt || token_type == TokenType::op_leq ||
       token_type == TokenType::op_gt || token_type == TokenType::op_geq) {

        expr_operator op;
        switch (token_type) {
            case TokenType::op_eq:
                op = expr_operator::EQ;
                break;
            case TokenType::op_neq:
                op = expr_operator::NEQ;
                break;
            case TokenType::op_lt:
                op = expr_operator::LT;
                break;
            case TokenType::op_leq:
                op = expr_operator::LEQ;
                break;
            case TokenType::op_gt:
                op = expr_operator::GT;
                break;
            case TokenType::op_geq:
                op = expr_operator::GEQ;
                break;
        }

        scanner_.next();
        auto snd_expr = simple_expression();

        return std::make_unique<ExpressionNode>(start,std::move(fst_expr),op,std::move(snd_expr));

    }
    else{
        return std::make_unique<ExpressionNode>(start,std::move(fst_expr));
    }


}

// Assignment -> Ident Selector ":=" Expression
std::unique_ptr<AssignmentNode> Parser::assignment() {
    logger_.info("Assignment");

    auto start = scanner_.peek()->start();
    auto id = ident();
    auto sel = selector();

    auto token = scanner_.peek();
    if(token->type() == TokenType::op_becomes){
        scanner_.next();
        auto expr = expression();
        return std::make_unique<AssignmentNode>(start,std::move(id),std::move(sel),std::move(expr));
    }

    return nullptr; //Syntax Error (Expected ":=", but got...) (Special Cases: Did you put "=" instead ":=" ? Did you forget the "=" after the ":")
}

// ActualParameters -> "(" (expression ("," expression)*)? ")"
std::unique_ptr<ActualParametersNode> Parser::actual_parameters() {
    logger_.info("Actual Parameters");
    auto token = scanner_.peek();

    auto actual_params = std::make_unique<ActualParametersNode>(token->start());

    if(token->type() == TokenType::lparen){
        scanner_.next();
        token = scanner_.peek();

        // Case: No expression
        if(token->type() == TokenType::rparen){
            scanner_.next();
            return actual_params; //Success
        }

        // Case: (At Least) One Expression
        actual_params->add_expression(expression());
        token = scanner_.peek();

        // Multiple Expressions
        while(token->type() == TokenType::comma){
            scanner_.next();
            actual_params->add_expression(expression());
            token = scanner_.peek();
        }

        if(token->type() == TokenType::rparen){
            scanner_.next();
            return actual_params;
        }
        else{
            return nullptr; // Syntax Error (Expected ")", but got...)
        }
    }

    return nullptr; // Syntax Error (Expected "(", but got...)

}

// ProcedureCall -> Ident Selector (ActualParameters)?
std::unique_ptr<ProcedureCallNode> Parser::procedure_call() {
    logger_.info("Procedure Call");

    auto start = scanner_.peek()->start();
    auto id = ident();
    auto sel = selector();

    // Check for parenthesis to see whether ActualParameters term exists
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::lparen){
        auto params = actual_parameters();
        return std::make_unique<ProcedureCallNode>(start,std::move(id),std::move(sel), std::move(params));
    }

    return std::make_unique<ProcedureCallNode>(start,std::move(id),std::move(sel));

}

// IfStatement -> "IF" expression "THEN" StatementSequence ("ELSIF" expression "THEN" StatementSequence)* ("ELSE" StatementSequence)? "END"
std::unique_ptr<IfStatementNode> Parser::if_statement() {
    logger_.info("If Statement");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_if){

        scanner_.next();
        auto condition = expression();
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::kw_then){
            scanner_.next();
            auto then = statement_sequence();
            token_type = scanner_.peek()->type();

            // CONSTRUCT NODE
            auto if_node = std::make_unique<IfStatementNode>(start,std::move(condition),std::move(then));

            // ELSE IF
            while(token_type == TokenType::kw_elsif){
                scanner_.next();
                auto elif_cond = expression();
                std::unique_ptr<StatementSequenceNode> elif_then;

                token_type = scanner_.peek()->type();

                if(token_type == TokenType::kw_then){
                    scanner_.next();
                    elif_then = statement_sequence();
                    if_node->add_else_if(std::move(elif_cond),std::move(elif_then));
                }else{
                    return nullptr; // Syntax Error (Expected "THEN" after "ELSIF")
                }

                token_type = scanner_.peek()->type();

            }

            // ELSE
            if(token_type == TokenType::kw_else){
                scanner_.next();
                if_node->add_else(statement_sequence());
                token_type = scanner_.peek()->type();
            }

            if(token_type == TokenType::kw_end){
                scanner_.next();
                return if_node; // All successful
            }


        }
        else{
            return nullptr; // Syntax Error (Expected "THEN" after IF)
        }

    }

    return nullptr; // Syntax Error (Expected "if", but got...)
}

// WhileStatement -> "WHILE" expression "DO" StatementSequence "END"
std::unique_ptr<WhileStatementNode> Parser::while_statement() {
    logger_.info("While Statement");

    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_while){
        scanner_.next();
        auto cond = expression();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::kw_do){
            scanner_.next();
            auto statements = statement_sequence();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::kw_end){
                scanner_.next();
                return std::make_unique<WhileStatementNode>(start, std::move(cond),std::move(statements)); // successful;
            }
            else{
                return nullptr; // Syntax Error (No "END" after While Statement)
            }
        }
        else{
            return nullptr; // Syntax Error (No "DO" after "WHILE")
        }
    }

    return nullptr; // Syntax Error (Expected "While", but got...)
}

// RepeatStatement -> "REPEAT" StatementSequence "UNTIL" expression
std::unique_ptr<RepeatStatementNode> Parser::repeat_statement() {
    logger_.info("Repeat Statement");

    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_repeat){
        scanner_.next();
        auto statements = statement_sequence();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::kw_until) {
            scanner_.next();
            auto cond = expression();
            return std::make_unique<RepeatStatementNode>(start,std::move(cond),std::move(statements));
        }
        else{
            return nullptr; // Syntax Error (No "UNTIL" after "REPEAT")
        }
    }

    return nullptr; // Syntax Error (Expected "REPEAT" but got...)
}

// Statement -> (assignment | Procedure Call | IfStatement | WhileStatement)
// Note: While not included in the CompilerConstruction book, this should contain "RepeatStatement" as well
std::unique_ptr<StatementNode>  Parser::statement() {

    logger_.info("Statement");

    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    // Check the next token to decide which Non-terminal "Statement" is derived into
    if(token_type == TokenType::const_ident){

        // If the token is an identifier then it could either correspond to an assignment or a procedure call
        // Thus Oberon-0 is not "really" an LL(1) language, but for the parser this makes little difference

        // Recall:
        //  assignment -> ident selector ":=" expression
        //  procedureCall -> ident selector [ActualParameters]?

        auto id = ident();
        auto sel = selector();

        token_type = scanner_.peek()->type();

        // Assignment
        if(token_type == TokenType::op_becomes){
            scanner_.next();
            auto expr = expression();
            return std::make_unique<AssignmentNode>(start,std::move(id),std::move(sel),std::move(expr));
        }

        // Procedure Call
        // Check for "(" to see whether the (ActualParameters) term exists
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::lparen){
            auto params = actual_parameters();
            return std::make_unique<ProcedureCallNode>(start,std::move(id),std::move(sel), std::move(params));
        }
        return std::make_unique<ProcedureCallNode>(start,std::move(id),std::move(sel));
    }

    // IfStatement
    if(token_type == TokenType::kw_if){
        return if_statement();
    }

    // WhileStatement
    if(token_type == TokenType::kw_while){
        return while_statement();
    }

    // RepeatStatement
    if(token_type == TokenType::kw_repeat){
        return repeat_statement();
    }

    return nullptr; // Syntax Error (Empty or Undefined Statement. Expected Identifier, "IF", "WHILE" or "REPEAT" but got...)

}

// StatementSequence -> Statement (";" statement)*
std::unique_ptr<StatementSequenceNode> Parser::statement_sequence() {
    logger_.info("Statement Sequence");
    auto start = scanner_.peek()->start();
    auto first = statement();

    auto sequence = std::make_unique<StatementSequenceNode>(start,std::move(first));

    auto token_type = scanner_.peek()->type();
    while(token_type == TokenType::semicolon){
        scanner_.next();
        sequence->add_statement(statement());
        token_type = scanner_.peek()->type();
    }

    return sequence;

}

// IdentList -> Ident ("," ident)*
std::unique_ptr<IdentListNode> Parser::ident_list() {
    logger_.info("Ident List");
    auto start = scanner_.peek()->start();
    auto id_list = std::make_unique<IdentListNode>(start,ident());

    auto token_type = scanner_.peek()->type();
    while(token_type == TokenType::comma){
        scanner_.next();
        id_list->add_identifier(ident());
        token_type = scanner_.peek()->type();
    }

    return id_list;
}

// FieldList -> (IdentList ":" type)?
std::unique_ptr<FieldListNode>  Parser::field_list() {
    logger_.info("Field List");
    auto start = scanner_.peek()->start();

    // Check Follows(FieldList) to decide whether this FieldList is empty or not
    // Follows(FieldList) = ; END
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::semicolon || token_type == TokenType::kw_end){
        return std::make_unique<FieldListNode>(start); // Field List empty, continue parsing in upper layer
    }

    auto idents = ident_list();
    token_type = scanner_.peek()->type();
    if(token_type == TokenType::colon){
        scanner_.next();
        auto idents_type = type();
        return std::make_unique<FieldListNode>(start,std::move(idents),std::move(idents_type));
    }

    return nullptr; // Syntax Error (Expected ":", but got...)

}

// Type -> Ident | ArrayType | RecordType
std::unique_ptr<TypeNode> Parser::type() {
    logger_.info("Type");
    auto token_type = scanner_.peek()->type();

    // We decide the according production by the next token

    // ArrayType
    if(token_type == TokenType::kw_array){
        return array_type();
    }

    // RecordType
    if(token_type == TokenType::kw_record){
        return record_type();
    }

    // Identifier
    return ident();

}

// ArrayType -> "ARRAY" expression "OF" type
std::unique_ptr<ArrayTypeNode> Parser::array_type() {
    logger_.info("Array Type");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_array){
        scanner_.next();
        auto expr = expression();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::kw_of){
            scanner_.next();
            auto arr_type = type();
            return std::make_unique<ArrayTypeNode>(start,std::move(expr),std::move(arr_type));
        }
        else{
            return nullptr; // Syntax Error (No "OF" after "ARRAY" in Type Description)
        }

    }

    return nullptr; // Syntax Error (Expected "ARRAY" but got, ...)
}

// RecordType = "RECORD" FieldList (";" FieldList)* "END"
std::unique_ptr<RecordTypeNode> Parser::record_type() {
    logger_.info("Record Type");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_record){
        scanner_.next();
        auto rec_type = std::make_unique<RecordTypeNode>(start,std::move(field_list()));

        token_type = scanner_.peek()->type();

        while (token_type == TokenType::semicolon){
            scanner_.next();
            rec_type->add_field_list(field_list());
            token_type = scanner_.peek()->type();
        }

        if(token_type == TokenType::kw_end){
            scanner_.next();
            return rec_type; // Success
        }
        else{
            return nullptr; // Syntax Error (No "END" after Record Type Declaration)
        }

    }

    return nullptr; // Syntax Error (Expected "RECORD", but got...)
}

// FPSection -> ("VAR")? IdentList ":" type
std::unique_ptr<FPSectionNode> Parser::fp_section() {
    logger_.info("FPSection");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    bool var_included = false;

    if(token_type == TokenType::kw_var){
        scanner_.next();
        var_included = true;
    }

    auto id_list = ident_list();

    token_type = scanner_.peek()->type();
    if(token_type == TokenType::colon){
        scanner_.next();
        auto var_type = type();
        return std::make_unique<FPSectionNode>(start,var_included,std::move(id_list),std::move(var_type));
    }

    return nullptr; // Syntax Error(Expected ":", but got...)

}

// FormalParameters -> "(" (FPSection (";" FPSection)*  )? ")"
std::unique_ptr<FormalParameterNode> Parser::formal_parameters() {
    logger_.info("Formal Parameters");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    auto formal_params = std::make_unique<FormalParameterNode>(start);

    if(token_type == TokenType::lparen){

        scanner_.next();
        token_type = scanner_.peek()->type();

        // Empty Parameters
        if(token_type == TokenType::rparen){
            scanner_.next();
            return formal_params; // Success
        }

        // FPSections
        formal_params->add_parameter_section(fp_section());
        token_type = scanner_.peek()->type();

        while(token_type == TokenType::semicolon){
            scanner_.next();
            formal_params->add_parameter_section(fp_section());
            token_type = scanner_.peek()->type();
        }

        if(token_type == TokenType::rparen){
            scanner_.next();
            return formal_params; // Success
        }
        else{
            return nullptr; // Syntax Error (Expected ")", but got...)
        }

    }

    return nullptr; // Syntax Error (Expected "(", but got...)
}

// ProcedureHeadingNode -> "PROCEDURE" Ident (FormalParameters)?
std::unique_ptr<ProcedureHeadingNode>  Parser::procedure_heading() {
    logger_.info("Procedure Heading");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_procedure){
        scanner_.next();
        auto id = ident();

        // To see whether FormalParameters follow, we check if an "(" follows
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::lparen){
            return std::make_unique<ProcedureHeadingNode>(start,std::move(id),formal_parameters());
        }

        return std::make_unique<ProcedureHeadingNode>(start,std::move(id));

    }

    return nullptr; // Syntax Error (Expected "PROCEDURE", but got...)
}

// ProcedureBodyNode -> declarations ("BEGIN" StatementSequence)? "END" ident
std::unique_ptr<ProcedureBodyNode> Parser::procedure_body() {
    logger_.info("Procedure Body");
    auto start = scanner_.peek()->start();
    auto declars = declarations();

    auto token_type = scanner_.peek()->type();

    // Statement Sequence
    std::unique_ptr<StatementSequenceNode> statements = nullptr;
    if(token_type == TokenType::kw_begin){
        scanner_.next();
        statements = statement_sequence();
        token_type = scanner_.peek()->type();
    }

    if(token_type == TokenType::kw_end){
        scanner_.next();
        return std::make_unique<ProcedureBodyNode>(start,std::move(declars),ident(),std::move(statements));
    }

    return nullptr; // Syntax Error (No "END" in Procedure Body) / (Expected "END", but got...)
}

// ProcedureDeclaration = ProcedureHeadingNode ";" ProcedureBodyNode
std::unique_ptr<ProcedureDeclarationNode> Parser::procedure_declaration() {
    logger_.info("Procedure Declaration");
    auto start = scanner_.peek()->start();
    auto heading = procedure_heading();

    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::semicolon){
        scanner_.next();
        auto body = procedure_body();
        return std::make_unique<ProcedureDeclarationNode>(start,std::move(heading),std::move(body));
    }

    return nullptr; // Syntax Error (Expected ";", but got...)

}

//  Declarations ->    ("CONST" (ident "=" expression ";")* )?
//                     ("TYPE"  (ident "=" type ";") *      )?
//                     ("VAR"   (identList ":" type ";")*   )?
//                     (ProcedureDeclaration ";"            )*
std::unique_ptr<DeclarationsNode> Parser::declarations() {
    logger_.info("Declarations");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    auto declarations_node = std::make_unique<DeclarationsNode>(start);

    // CONST declarations
    if(token_type == TokenType::kw_const){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // (ident "=" expression ";")*
        while(token_type == TokenType::const_ident){
            auto id = ident();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::op_eq){
                scanner_.next();
                auto expr = expression();

                token_type = scanner_.peek()->type();
                if(token_type == TokenType::semicolon){
                    scanner_.next();
                    token_type = scanner_.peek()->type();
                    declarations_node->add_constant(std::move(id),std::move(expr));
                }else{
                    return nullptr; // Syntax Error (Expected ";", but got...)
                }


            }
            else{
                return nullptr; // Syntax Error (Expected "=" after identifier)
            }


        }
    }

    // TYPE declarations
    if(token_type == TokenType::kw_type){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // (ident "=" type ";")*
        while (token_type == TokenType::const_ident){
            auto id = ident();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::op_eq){
                scanner_.next();
                auto id_type = type();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::semicolon){
                    scanner_.next();
                    token_type = scanner_.peek()->type();
                    declarations_node->add_type(std::move(id),std::move(id_type));
                }else{
                    return nullptr; // Syntax error (Expected ";", but got...)
                }

            }
            else{
                return nullptr; // Syntax Error (Expected "=" after identifier, but got...)
            }


        }

    }

    // VAR declarations
    if(token_type == TokenType::kw_var){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // (identList ":" type ";")*
        while(token_type == TokenType::const_ident){
            auto id_list = ident_list();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::colon){
                scanner_.next();
                auto id_type = type();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::semicolon){
                    scanner_.next();
                    token_type = scanner_.peek()->type();
                    declarations_node->add_var(std::move(id_list),std::move(id_type));
                }else{
                    return nullptr; // Syntax Error (Expected ";", but got...)
                }

            }else{
                return nullptr; // Syntax Error (Expected ":" after identifiers, but got...)
            }

        }

    }

    // Procedure Declarations
    while(token_type == TokenType::kw_procedure){
        auto procedure = procedure_declaration();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::semicolon){
            scanner_.next();
            token_type = scanner_.peek()->type();
            declarations_node->add_procedure(std::move(procedure));
        }else{
            return nullptr; // Syntax Error (Expected ";" after Procedure Declarations, but got...)
        }

    }

    return declarations_node;

}

// Module -> "Module" ident ";" declarations ("BEGIN" StatementSequence)? "END" ident "."
std::unique_ptr<ModuleNode> Parser::module() {
    logger_.info("Module");
    auto start = scanner_.peek()->start();
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_module){
        scanner_.next();
        auto module_name_begin = ident();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::semicolon){
            scanner_.next();
            auto declars = declarations();
            token_type = scanner_.peek()->type();

            // Statement Sequence
            std::unique_ptr<StatementSequenceNode> statements = nullptr;
            if(token_type == TokenType::kw_begin){
                scanner_.next();
                statements = statement_sequence();
                token_type = scanner_.peek()->type();
            }

            // "END" ident "."
            if(token_type == TokenType::kw_end){
                scanner_.next();
                auto module_name_end = ident();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::period){
                    scanner_.next();
                    return std::make_unique<ModuleNode>(start,std::move(module_name_begin),std::move(declars),std::move(statements),std::move(module_name_end));
                }

                return nullptr; // Syntax Error (Expected "." at the end of module, but got...)

            }

            return nullptr; // Syntax Error (No "END" at the end of Module)


        }else{
            return nullptr; // Syntax Error (Expected ";", but got...)
        }
    }

    return nullptr; // Syntax Error (Expected keyword "Module", but got...)
}

std::unique_ptr<ModuleNode> Parser::parse() {
    return module();
}
