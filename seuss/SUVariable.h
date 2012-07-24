//
//  SUVariable.h
//  seuss
//
//  Created by Cory Kilger on 7/20/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUVariable_h
#define seuss_SUVariable_h

struct seuss_type;
struct seuss_list;

typedef struct seuss_variable SUVariable;

SUVariable * SUVariableCreate(struct seuss_list * name);
struct seuss_list * SUVariableGetName(SUVariable * variable);
void * SUVariableGetValue(SUVariable * variable);
void SUVariableSetValue(SUVariable * variable, void * value);
int SUTypeIsVariable(struct seuss_type * type);
struct seuss_list * SUVariableCreateBuiltins();

#endif
