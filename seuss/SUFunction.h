//
//  SUFunction.h
//  seuss
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUFunction_h
#define seuss_SUFunction_h

struct seuss_iterator;
struct seuss_token;
struct seuss_list;
struct seuss_program;

typedef struct seuss_function SUFunction;

SUFunction * SUFunctionCreate(struct seuss_list * functions, struct seuss_iterator * iterator, struct seuss_token * token, struct seuss_list * errors);
struct seuss_list * SUFunctionGetSignature(SUFunction * function);
struct seuss_list * SUFunctionCreateBuiltins();
struct seuss_list * SUFunctionCreateParametersForStatementTokens(SUFunction * function, struct seuss_list * tokens);
void SUFunctionExecute(struct seuss_program * program, SUFunction * function, struct seuss_list * parameters);

#endif
