/*
 * Parser of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/2/18.
 */

#ifndef OBERON0C_PARSER_H
#define OBERON0C_PARSER_H


#include <string>
#include "scanner/Scanner.h"
#include "ast/Node.h"

using std::string;

class Parser {

private:
    Scanner &scanner_;
    Logger &logger_;

    const string ident();
    void integer();
    void selector();
    void number();
    void factor();
    void term();

    void simple_expression();
    void expression();
    void assignment();

    void actual_parameters();
    void procedure_call();

    void if_statement();
    void while_statement();
    void repeat_statement();
    void statement();
    void statement_sequence();

    void ident_list();
    void field_list();

    void type();
    void array_type();
    void record_type();

    void fp_section();
    void formal_parameters();

    void procedure_heading();
    void procedure_body();
    void procedure_declaration();
    void declarations();

    void module();

public:
    explicit Parser(Scanner &scanner, Logger &logger) : scanner_(scanner), logger_(logger) {};
    ~Parser() = default;
    void parse();

};


#endif //OBERON0C_PARSER_H
