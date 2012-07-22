//
//  SUProgram.h
//  suess
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUProgram_h
#define suess_SUProgram_h

struct suess_iterator;
struct suess_list;
struct suess_string;

typedef void(*SUProgramWriteCallback)(struct suess_string *, void *);
typedef struct suess_string *(*SUProgramReadCallback)(void *);

typedef struct suess_program SUProgram;

SUProgram * SUProgramCreate(struct suess_list * tokens, struct suess_list * errors);
struct suess_list * SUProgramGetFunctions(SUProgram * program);
struct suess_list * SUProgramGetStatements(SUProgram * program);
struct suess_list * SUProgramGetVariables(SUProgram * program);
void SUProgramExecute(SUProgram * program);
void SUProgramSetWriteCallback(SUProgram * program, SUProgramWriteCallback writeCallback, void * userData);
void SUProgramSetReadCallback(SUProgram * program, SUProgramReadCallback readCallback, void * userData);
SUProgramWriteCallback SUProgramGetWriteCallback(SUProgram * program);
SUProgramReadCallback SUProgramGetReadCallback(SUProgram * program);
void * SUProgramGetWriteData(SUProgram * program);
void * SUProgramGetReadData(SUProgram * program);

#endif
