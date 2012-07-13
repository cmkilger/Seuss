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

typedef struct suess_function SUFunction;

SUFunction * SUFunctionCreate(struct suess_iterator * iterator, char ** error);

#endif
