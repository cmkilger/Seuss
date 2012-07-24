//
//  SUStatement.h
//  seuss
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUStatement_h
#define seuss_SUStatement_h

struct seuss_list;
struct seuss_iterator;
struct seuss_token;
struct seuss_program;
struct seuss_function;

typedef struct seuss_statement SUStatement;

SUStatement * SUStatementCreate(struct seuss_list * functions, struct seuss_list * variables, struct seuss_iterator * iterator, struct seuss_token * currentToken, struct seuss_list * errors);
struct seuss_function * SUStatementGetFunction(SUStatement * statement);
struct seuss_list * SUStatementGetParameters(SUStatement * statement);
void SUStatementExecute(struct seuss_program * program, SUStatement * statement);

#endif
