// Implementation of the Parser used by the Oberon0 Compiler
//
// Created by Max Galetskiy on 16.11.2024.
//

#include "Parser.h"

// ident -> letter (letter | digit)*  (already recognized by the scanner in full)
void Parser::ident() {
    logger_.info("Ident");
    auto token = scanner_.peek();
    if(token->type() == TokenType::const_ident){
        scanner_.next();
        return;                                    // success
    }

    return; // Syntax Error (Expected Identifier, but got)

}

// integer -> digit (digit)*  (already recognized by the scanner in full)
void Parser::integer() {
    logger_.info("Integer");
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::int_literal || token_type == TokenType::short_literal || token_type == TokenType::long_literal){
        scanner_.next();
        return; // Success
    }

    return; // Syntax Error (Expected Integer, but got)
}

// number -> integer
void Parser::number() {
    integer();
}

// selector -> ("." ident | "[" expression "]" )*
void Parser::selector() {
    logger_.info("Selector");
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
                return; // Syntax Error (Expected ")" but got)
            }
        }

        token_type = scanner_.peek()->type();

    }

}

// factor -> ident selector | number | "(" expression ")" | "~" factor
void Parser::factor() {
    logger_.info("Factor");
    auto token = scanner_.peek();
    switch (token->type()) {

        case TokenType::const_ident:
            ident();
            selector();
            return;
        case TokenType::int_literal:
        case TokenType::short_literal:
        case TokenType::long_literal:
            number();
            return;
        case TokenType::lparen:
            scanner_.next();
            expression();
            token = scanner_.peek();
            if(token->type() == TokenType::rparen){
                scanner_.next();
                return;
                // successful
            }
            return;
        case TokenType::op_not:
            scanner_.next();
            factor();
            return;                 // successful
        default:
            // syntax error (Expected identifier, integer, "(" or "~", but got)
            return;
    }

}

// term -> factor (("*" | "DIV" | "MOD" | "&") factor)*
void Parser::term() {
    logger_.info("Term");
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
    logger_.info("Simple Expression");
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
    logger_.info("Expression");
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
    logger_.info("Assignment");
    ident();
    selector();

    auto token = scanner_.peek();
    if(token->type() == TokenType::op_becomes){
        scanner_.next();
        expression();
        return;
    }

    return; //Syntax Error (Expected ":=", but got...) (Special Cases: Did you put "=" instead ":=" ? Did you forget the "=" after the ":")
}

// ActualParameters -> "(" (expression ("," expression)*)? ")"
void Parser::actual_parameters() {
    logger_.info("Actual Parameters");
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
            // Syntax Error (Expected ")", but got...)
        }
    }

    return; // Syntax Error (Expected "(", but got...)

}

// ProcedureCall -> Ident Selector (ActualParameters)?
void Parser::procedure_call() {
    logger_.info("Procedure Call");
    ident();
    selector();

    // Check for parenthesis to see whether ActualParameters term exists
    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::lparen){
        actual_parameters();
    }

}

// IfStatement -> "IF" expression "THEN" StatementSequence ("ELSIF" expression "THEN" StatementSequence)* ("ELSE" StatementSequence)? "END"
void Parser::if_statement() {
    logger_.info("If Statement");
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
                    return; // Syntax Error (Expected "THEN" after "ELSIF")
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
        else{
            return; // Syntax Error (Expected "THEN" after IF)
        }

    }

    return; // Syntax Error (Expected "if", but got...)
}

// WhileStatement -> "WHILE" expression "DO" StatementSequence "END"
void Parser::while_statement() {
    logger_.info("While Statement");
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
            else{
                return; // Syntax Error (No "END" after While Statement)
            }
        }
        else{
            return; // Syntax Error (No "DO" after "WHILE")
        }
    }

    return; // Syntax Error (Expected "While", but got...)
}

// RepeatStatement -> "REPEAT" StatementSequence "UNTIL" expression
void Parser::repeat_statement() {
    logger_.info("Repeat Statement");
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
        else{
            return; // Syntax Error (No "UNTIL" after "REPEAT")
        }
    }

    return; // Syntax Error (Expected "REPEAT" but got...)
}

// Statement -> (assignment | Procedure Call | IfStatement | WhileStatement)
// Note: While not included in the CompilerConstruction book, this should contain "RepeatStatement" as well
void Parser::statement() {

    logger_.info("Statement");

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
        // Check for "(" to see whether the (ActualParameters) term exists
        token_type = scanner_.peek()->type();
        if(token_type == TokenType::lparen){
            actual_parameters();
        }
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

    return; // Syntax Error (Empty or Undefined Statement. Expected Identifier, "IF", "WHILE" or "REPEAT" but got...)

}

// StatementSequence -> Statement (";" statement)*
void Parser::statement_sequence() {
    logger_.info("Statement Sequence");
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
    logger_.info("Ident List");
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
    logger_.info("Field List");
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

    return; // Syntax Error (Expected ":", but got...)

}

// Type -> Ident | ArrayType | RecordType
void Parser::type() {
    logger_.info("Type");
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
    logger_.info("Array Type");
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
            return; // Syntax Error (No "OF" after "ARRAY" in Type Description)
        }

    }

    return; // Syntax Error (Expected "ARRAY" but got, ...)
}

// RecordType = "RECORD" FieldList (";" FieldList)* "END"
void Parser::record_type() {
    logger_.info("Record Type");
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
            return; // Syntax Error (No "END" after Record Type Declaration)
        }

    }

    return; // Syntax Error (Expected "RECORD", but got...)
}

// FPSection -> ("VAR")? IdentList ":" type
void Parser::fp_section() {
    logger_.info("FPSection");
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

    return; // Syntax Error(Expected ":", but got...)

}

// FormalParameters -> "(" (FPSection (";" FPSection)*  )? ")"
void Parser::formal_parameters() {
    logger_.info("Formal Parameters");
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
            return; // Syntax Error (Expected ")", but got...)
        }

    }

    return; // Syntax Error (Expected "(", but got...)
}

// ProcedureHeadingNode -> "PROCEDURE" Ident (FormalParameters)?
void Parser::procedure_heading() {
    logger_.info("Procedure Heading");
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

    return; // Syntax Error (Expected "PROCEDURE", but got...)
}

// ProcedureBodyNode -> declarations ("BEGIN" StatementSequence)? "END" ident
void Parser::procedure_body() {
    logger_.info("Procedure Body");
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

    return; // Syntax Error (No "END" in Procedure Body) / (Expected "END", but got...)
}

// ProcedureDeclaration = ProcedureHeadingNode ";" ProcedureBodyNode
void Parser::procedure_declaration() {
    logger_.info("Procedure Declaration");
    procedure_heading();

    auto token_type = scanner_.peek()->type();
    if(token_type == TokenType::semicolon){
        scanner_.next();
        procedure_body();
        return;
    }

    return; // Syntax Error (Expected ";", but got...)

}

//  Declarations ->    ("CONST" (ident "=" expression ";")* )?
//                     ("TYPE"  (ident "=" type ";") *      )?
//                     ("VAR"   (identList ":" type ";")*   )?
//                     (ProcedureDeclaration ";"            )*
void Parser::declarations() {
    logger_.info("Declarations");
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
                    return; // Syntax Error (Expected ";", but got...)
                }


            }
            else{
                return; // Syntax Error (Expected "=" after identifier)
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
                    return; // Syntax error (Expected ";", but got...)
                }

            }
            else{
                return; // Syntax Error (Expected "=" after identifier, but got...)
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
                    return; // Syntax Error (Expected ";", but got...)
                }

            }else{
                return; // Syntax Error (Expected ":" after identifiers, but got...)
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
            return; // Syntax Error (Expected ";" after Procedure Declarations, but got...)
        }

    }

}

// Module -> "Module" ident ";" declarations ("BEGIN" StatementSequence)? "END" ident "."
void Parser::module() {
    logger_.info("Module");
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

                return; // Syntax Error (Expected "." at the end of module, but got...)

            }

            return; // Syntax Error (No "END" at the end of Module)


        }else{
            return; // Syntax Error (Expected ";", but got...)
        }
    }

    return; // Syntax Error (Expected keyword "Module", but got...)
}

void Parser::parse() {
    module();
}
