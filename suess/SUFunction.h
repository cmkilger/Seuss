//
//  SUFunction.h
//  suess
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUFunction_h
#define suess_SUFunction_h

struct suess_iterator;
struct suess_token;
struct suess_list;
struct suess_program;

typedef struct suess_function SUFunction;

SUFunction * SUFunctionCreate(struct suess_list * functions, struct suess_iterator * iterator, struct suess_token * token, struct suess_list * errors);
struct suess_list * SUFunctionCreateBuiltins();
struct suess_list * SUFunctionCreateParametersForStatementTokens(SUFunction * function, struct suess_list * tokens);
void SUFunctionExecute(struct suess_program * program, SUFunction * function, struct suess_list * parameters);

#endif
