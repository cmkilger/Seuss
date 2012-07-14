//
//  SUProgram.c
//  suess
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUProgram.h"
#include "SUTypeInternal.h"
#include "SUList.h"
#include "SUFunction.h"
#include "SUIterator.h"
#include "SUTokenizer.h"
#include "SUStatement.h"

struct suess_program {
    SUType __base;
    SUList * functions;
    SUList * statements;
};

void suess_program_free(SUTypeRef type) {
    SUProgram * program = type;
    if (program->functions)
        SURelease(program->functions);
    if (program->statements)
        SURelease(program->statements);
    suess_free(type);
}

SUProgram * SUProgramCreate(SUIterator * iterator, char ** error) {
    SUList * functions = SUListCreate();
    SUList * statements = SUListCreate();
    
    SUToken * token = NULL;
    while ((token = SUIteratorNext(iterator))) {
        switch (SUTokenGetType(token)) {
            case SUTokenTypeStartFunctionDefinition: {
                SUFunction * function = SUFunctionCreate(functions, iterator, error);
                if (function) {
                    SUListAddValue(functions, function, 1);
                    SURelease(function);
                }
            } break;
                
            case SUTokenTypeWord: {
                SUStatement * statement = SUStatementCreate(functions, iterator, error);
                if (statement) {
                    SUListAddValue(statements, statement, 1);
                    SURelease(statement);
                }
            } break;
                
            default: {
//                token = NULL;
                if (error)
                    *error = "Unable to parse!";
            } break;
        }
    }
    
    SUProgram * program = malloc(sizeof(SUProgram));
    SUInitialize(program, NULL, NULL, suess_program_free);
    program->functions = functions;
    program->statements = statements;
    
    return program;
}
