//
//  SUVariable.h
//  suess
//
//  Created by Cory Kilger on 7/20/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUVariable_h
#define suess_SUVariable_h

struct suess_type;
struct suess_list;

typedef struct suess_variable SUVariable;

SUVariable * SUVariableCreate(struct suess_list * name);
struct suess_list * SUVariableGetName(SUVariable * variable);
void * SUVariableGetValue(SUVariable * variable);
void SUVariableSetValue(SUVariable * variable, void * value);
int SUTypeIsVariable(struct suess_type * type);
struct suess_list * SUVariableCreateBuiltins();

#endif
