//
//  SUProgram.h
//  seuss
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUProgram_h
#define seuss_SUProgram_h

struct seuss_iterator;
struct seuss_list;
struct seuss_string;

typedef void(*SUProgramWriteCallback)(struct seuss_string *, void *);
typedef struct seuss_string *(*SUProgramReadCallback)(void *);

typedef struct seuss_program SUProgram;

SUProgram * SUProgramCreate(struct seuss_list * tokens, struct seuss_list * errors);
struct seuss_list * SUProgramGetFunctions(SUProgram * program);
struct seuss_list * SUProgramGetStatements(SUProgram * program);
struct seuss_list * SUProgramGetVariables(SUProgram * program);
void SUProgramExecute(SUProgram * program);
void SUProgramSetWriteCallback(SUProgram * program, SUProgramWriteCallback writeCallback, void * userData);
void SUProgramSetReadCallback(SUProgram * program, SUProgramReadCallback readCallback, void * userData);
SUProgramWriteCallback SUProgramGetWriteCallback(SUProgram * program);
SUProgramReadCallback SUProgramGetReadCallback(SUProgram * program);
void * SUProgramGetWriteData(SUProgram * program);
void * SUProgramGetReadData(SUProgram * program);

#endif
