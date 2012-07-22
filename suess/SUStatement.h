//
//  SUStatement.h
//  suess
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUStatement_h
#define suess_SUStatement_h

struct suess_list;
struct suess_iterator;
struct suess_token;
struct suess_program;

typedef struct suess_statement SUStatement;

SUStatement * SUStatementCreate(struct suess_list * functions, struct suess_list * variables, struct suess_iterator * iterator, struct suess_token * currentToken, struct suess_list * errors);
void SUStatementExecute(struct suess_program * program, SUStatement * statement);

#endif
