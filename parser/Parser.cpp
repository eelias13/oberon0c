// Implementation of the Parser used by the Oberon0 Compiler
//
// Created by Max Galetskiy on 16.11.2024.
//

#include "Parser.h"

// ident -> letter (letter | digit)*  (already recognized by the scanner in full)
const string Parser::ident() {
    auto token = scanner_.peek();
    if(token->type() == TokenType::const_ident){
        return nullptr;                                    // success
    }

    return nullptr; // Syntax Error

}

// integer -> digit (digit)*  (already recognized by the scanner in full)
void Parser::integer() {
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::int_literal){
        return; // Success
    }

    return; // Syntax Error
}

// number -> integer
void Parser::number() {
    integer();
}

// selector -> ("." ident | "[" expression "]" )*
void Parser::selector() {
    auto token_type = scanner_.peek()->type();

    while(token_type == TokenType::period || token_type == TokenType::lbrack){

        if(token_type == TokenType::period){
            scanner_.next();
            ident();
        }
        else{
            scanner_.next();
            expression();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::rbrack){
                scanner_.next();
            }else{
                return; // Syntax Error
            }
        }

        token_type = scanner_.peek()->type();

    }

}

// factor -> ident selector | number | "(" expression ")" | "~" factor
void Parser::factor() {
    auto token = scanner_.peek();
    switch (token->type()) {

        case TokenType::const_ident:
            ident();
            selector();
            break;
        case TokenType::int_literal:
            number();
            break;
        case TokenType::lparen:
            scanner_.next();
            expression();
            token = scanner_.peek();
            if(token->type() == TokenType::rparen){
                scanner_.next();
                return;
                // successful
            }
            break;
        case TokenType::op_not:
            scanner_.next();
            factor();
            return;                 // successful
        default:
            // syntax error
            break;
    }

    return; // report Syntax Error
}

// term -> factor (("*" | "DIV" | "MOD" | "&") factor)*
void Parser::term() {
    factor();
    auto token_type = scanner_.peek()->type();
    while(token_type == TokenType::op_times || token_type == TokenType::op_div ||
          token_type == TokenType::op_mod   || token_type == TokenType::op_and) {

        scanner_.next();
        factor();
        token_type = scanner_.peek()->type();
    }

    return;

}

// SimpleExpression -> ("+" | "-" | €) term (("+"|"-"|"OR") term)*
void Parser::simple_expression() {
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::op_plus || token_type == TokenType::op_minus){
        scanner_.next();
    }
    term();

    token_type = scanner_.peek()->type();
    while(token_type == TokenType::op_plus || token_type == TokenType::op_minus || token_type == TokenType::op_or){

        scanner_.next();
        term();
        token_type = scanner_.peek()->type();
    }

    return;

}

// Expression -> SimpleExpression (("="|"#"|"<"|"<="|">"|">=") SimpleExpression)?
void Parser::expression() {
    simple_expression();

    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::op_eq || token_type == TokenType::op_neq ||
       token_type == TokenType::op_lt || token_type == TokenType::op_leq ||
       token_type == TokenType::op_gt || token_type == TokenType::op_geq) {

        scanner_.next();
        simple_expression();
    }


}

// Assignment -> Ident Selector ":=" Expression
void Parser::assignment() {
    ident();
    selector();

    auto token = scanner_.peek();
    if(token->type() == TokenType::op_becomes){
        scanner_.next();
        expression();
        return;
    }

    return; //Syntax Error
}

// ActualParameters -> "(" (expression ("," expression)*)? ")"
void Parser::actual_parameters() {
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::lparen){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // Case: No expression
        if(token_type == TokenType::rparen){
            scanner_.next();
            return; //Success
        }

        // Case: (At Least) One Expression
        expression();
        token_type = scanner_.peek()->type();

        // Multiple Expressions
        while(token_type == TokenType::comma){
            scanner_.next();
            expression();
            token_type = scanner_.peek()->type();
        }

        if(token_type == TokenType::rparen){
            scanner_.next();
            return;
        }
        else{
            // Syntax Error
        }
    }

    return; // Syntax Error

}

// ProcedureCall -> Ident Selector (ActualParameters)?
void Parser::procedure_call() {
    ident();
    selector();

    // Check FOLLOWS(ProcedureCall) to see whether the (ActualParameters) term exists
    // FOLLOWS(ProcedureCall) = ; END ELSE ELSIF UNTIL
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::semicolon || token_type == TokenType::kw_end ||
       token_type == TokenType::kw_else   || token_type == TokenType::kw_elsif ||
       token_type == TokenType::kw_until){

        return; // Continue Parsing in the upper level

    }

    // Otherwise try to parse the ActualParameters
    actual_parameters();
}

// IfStatement -> "IF" expression "THEN" StatementSequence ("ELSIF" expression "THEN" StatementSequence)* ("ELSE" StatementSequence)? "END"
void Parser::if_statement() {
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::kw_if){

        scanner_.next();
        expression();
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::kw_then){
            scanner_.next();
            statement_sequence();
            token_type = scanner_.peek()->type();

            // ELSE IF
            while(token_type == TokenType::kw_elsif){
                scanner_.next();
                expression();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::kw_then){
                    scanner_.next();
                    statement_sequence();
                }else{
                    return; // Syntax Error
                }

                token_type = scanner_.peek()->type();

            }

            // ELSE
            if(token_type == TokenType::kw_else){
                scanner_.next();
                statement_sequence();
                token_type = scanner_.peek()->type();
            }

            if(token_type == TokenType::kw_end){
                scanner_.next();
                return; // All successful
            }


        }

    }

    return; // Syntax Error
}

// WhileStatement -> "WHILE" expression "DO" StatementSequence "END"
void Parser::while_statement() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_while){
        scanner_.next();
        expression();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::kw_do){
            scanner_.next();
            statement_sequence();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::kw_end){
                scanner_.next();
                return; // successful;
            }
        }
    }

    return; // Syntax Error
}

// RepeatStatement -> "REPEAT" StatementSequence "UNTIL" expression
void Parser::repeat_statement() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_repeat){
        scanner_.next();
        statement_sequence();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::kw_until) {
            scanner_.next();
            expression();
            return;
        }
    }

    return; // Syntax Error
}

// Statement -> (assignment | Procedure Call | IfStatement | WhileStatement)
// Note: While not included in the CompilerConstruction book, this should contain "RepeatStatement" as well?
void Parser::statement() {

    auto token_type = scanner_.peek()->type();

    // Check the next token to decide which Non-terminal "Statement" is derived into
    if(token_type == TokenType::const_ident){

        // If the token is an identifier then it could either correspond to an assignment or a procedure call
        // Thus Oberon-0 is not "really" an LL(1) language, but for the parser this makes little difference

        // Recall:
        //  assignment -> ident selector ":=" expression
        //  procedureCall -> ident selector [ActualParameters]?

        ident();
        selector();

        token_type = scanner_.peek()->type();

        // Assignment
        if(token_type == TokenType::op_becomes){
            scanner_.next();
            expression();
            return;
        }

        // Procedure Call
        // Check FOLLOWS(ProcedureCall) to see whether the (ActualParameters) term exists
        // FOLLOWS(ProcedureCall) = ; END ELSE ELSIF UNTIL
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::semicolon || token_type == TokenType::kw_end ||
           token_type == TokenType::kw_else   || token_type == TokenType::kw_elsif ||
           token_type == TokenType::kw_until){

            return; // Continue Parsing in the upper level

        }

        // Otherwise try to parse the ActualParameters
        actual_parameters();
        return;
    }

    // IfStatement
    if(token_type == TokenType::kw_if){
        if_statement();
        return;
    }

    // WhileStatement
    if(token_type == TokenType::kw_while){
        while_statement();
        return;
    }

    // RepeatStatement
    if(token_type == TokenType::kw_repeat){
        repeat_statement();
        return;
    }

    return; // Syntax Error

}

// StatementSequence -> Statement (";" statement)*
void Parser::statement_sequence() {
    statement();

    auto token_type = scanner_.peek()->type();
    while(token_type == TokenType::semicolon){
        scanner_.next();
        statement();
        token_type = scanner_.peek()->type();
    }

}

// IdentList -> Ident ("," ident)*
void Parser::ident_list() {
    ident();

    auto token_type = scanner_.peek()->type();
    while(token_type == TokenType::comma){
        scanner_.next();
        ident();
        token_type = scanner_.peek()->type();
    }
}

// FieldList -> (IdentList ":" type)?
void Parser::field_list() {

    // Check Follows(FieldList) to decide whether this FieldList is empty or not
    // Follows(FieldList) = ; END

    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::semicolon || token_type == TokenType::kw_end){
        return; // Field List empty, continue parsing in upper layer
    }

    ident_list();
    token_type = scanner_.peek()->type();
    if(token_type == TokenType::colon){
        scanner_.next();
        type();
        return;
    }

    return; // Syntax Error

}

// Type -> Ident | ArrayType | RecordType
void Parser::type() {

    auto token_type = scanner_.peek()->type();

    // We decide the according production by the next token

    // ArrayType
    if(token_type == TokenType::kw_array){
        array_type();
        return;
    }

    // RecordType
    if(token_type == TokenType::kw_record){
        record_type();
        return;
    }

    // Identifier
    ident();

}

// ArrayType -> "ARRAY" expression "OF" type
void Parser::array_type() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_array){
        scanner_.next();
        expression();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::kw_of){
            scanner_.next();
            type();
            return;
        }
        else{
            return; // Syntax Error
        }

    }

    return; // Syntax Error
}

// RecordType = "RECORD" FieldList (";" FieldList)* "END"
void Parser::record_type() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_record){
        scanner_.next();
        field_list();
        token_type = scanner_.peek()->type();

        while (token_type == TokenType::semicolon){
            scanner_.next();
            field_list();
            token_type = scanner_.peek()->type();
        }

        if(token_type == TokenType::kw_end){
            scanner_.next();
            return; // Success
        }
        else{
            return; // Syntax Error
        }

    }

    return; // Syntax Error
}

// FPSection -> ("VAR")? IdentList ":" type
void Parser::fp_section() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_var){
        scanner_.next();
    }

    ident_list();

    token_type = scanner_.peek()->type();
    if(token_type == TokenType::colon){
        scanner_.next();
        type();
        return;
    }

    return; // Syntax Error

}

// FormalParameters -> "(" (FPSection (";" FPSection)*  )? ")"
void Parser::formal_parameters() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::lparen){

        scanner_.next();
        token_type = scanner_.peek()->type();

        // Empty Parameters
        if(token_type == TokenType::rparen){
            scanner_.next();
            return; // Success
        }

        // FPSections
        fp_section();
        token_type = scanner_.peek()->type();

        while(token_type == TokenType::semicolon){
            scanner_.next();
            fp_section();
            token_type = scanner_.peek()->type();
        }

        if(token_type == TokenType::rparen){
            scanner_.next();
            return; // Success
        }
        else{
            return; // Syntax Error
        }

    }

    return; // Syntax Error
}

// ProcedureHeading -> "PROCEDURE" Ident (FormalParameters)?
void Parser::procedure_heading() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_procedure){
        scanner_.next();
        ident();

        // To see whether FormalParameters follow, we check if an "(" follows
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::lparen){
            formal_parameters();
        }

        return; // Success

    }

    return; // Syntax Error
}

// ProcedureBody -> declarations ("BEGIN" StatementSequence)? "END" ident
void Parser::procedure_body() {
    declarations();

    auto token_type = scanner_.peek()->type();

    // Statement Sequence
    if(token_type == TokenType::kw_begin){
        scanner_.next();
        statement_sequence();
        token_type = scanner_.peek()->type();
    }

    if(token_type == TokenType::kw_end){
        scanner_.next();
        ident();
        return;
    }

    return; // Syntax Error
}

// ProcedureDeclaration = ProcedureHeading ";" ProcedureBody
void Parser::procedure_declaration() {

    procedure_heading();

    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::semicolon){
        scanner_.next();
        procedure_body();
        return;
    }

    return; // Syntax Error

}

//  Declarations ->    ("CONST" (ident "=" expression ";")* )?
//                     ("TYPE"  (ident "=" type ";") *      )?
//                     ("VAR"   (identList ":" type ";")*   )?
//                     (ProcedureDeclaration ";"            )*
void Parser::declarations() {
    auto token_type = scanner_.peek()->type();

    // CONST declarations
    if(token_type == TokenType::kw_const){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // (ident "=" expression ";")*
        while(token_type == TokenType::const_ident){
            ident();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::op_eq){
                scanner_.next();
                expression();

                token_type = scanner_.peek()->type();
                if(token_type == TokenType::semicolon){
                    scanner_.next();
                    token_type = scanner_.peek()->type();
                }else{
                    return; // Syntax Error
                }


            }
            else{
                return; // Syntax Error
            }


        }
    }

    // TYPE declarations
    if(token_type == TokenType::kw_type){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // (ident "=" type ";")*
        while (token_type == TokenType::const_ident){
            ident();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::op_eq){
                scanner_.next();
                type();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::semicolon){
                    scanner_.next();
                    token_type = scanner_.peek()->type();
                }else{
                    return; // Syntax error
                }

            }
            else{
                return; // Syntax Error
            }


        }

    }

    // VAR declarations
    if(token_type == TokenType::kw_var){
        scanner_.next();
        token_type = scanner_.peek()->type();

        // (identList ":" type ";")*
        while(token_type == TokenType::const_ident){
            ident_list();
            token_type = scanner_.peek()->type();

            if(token_type == TokenType::colon){
                scanner_.next();
                type();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::semicolon){
                    scanner_.next();
                    token_type = scanner_.peek()->type();
                }else{
                    return; // Syntax Error
                }

            }else{
                return; // Syntax Error
            }

        }

    }

    // Procedure Declarations
    while(token_type == TokenType::kw_procedure){
        procedure_declaration();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::semicolon){
            scanner_.next();
            token_type = scanner_.peek()->type();
        }else{
            return; // Syntax Error
        }

    }

}

// Module -> "Module" ident ";" declarations ("BEGIN" StatementSequence)? "END" ident "."
void Parser::module() {
    auto token_type = scanner_.peek()->type();

    if(token_type == TokenType::kw_module){
        scanner_.next();
        ident();
        token_type = scanner_.peek()->type();

        if(token_type == TokenType::semicolon){
            scanner_.next();
            declarations();
            token_type = scanner_.peek()->type();

            // Statement Sequence
            if(token_type == TokenType::kw_begin){
                scanner_.next();
                statement_sequence();
                token_type = scanner_.peek()->type();
            }

            // "END" ident "."
            if(token_type == TokenType::kw_end){
                scanner_.next();
                ident();
                token_type = scanner_.peek()->type();

                if(token_type == TokenType::period){
                    scanner_.next();
                    return;
                }

                return; // Syntax Error

            }

            return; // Syntax Error


        }else{
            return; // Syntax Error
        }
    }

    return; // Syntax Error
}















