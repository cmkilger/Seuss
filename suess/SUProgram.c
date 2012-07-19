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
#include "SUError.h"

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

SUProgram * SUProgramCreate(SUList * tokens, SUList * errors) {
    SUIterator * iterator = SUListCreateIterator(tokens);
    
    SUList * functions = SUListCreate();
    SUList * statements = SUListCreate();
    
    SUList * variables = SUListCreate();
    SUToken * token = NULL;
    while ((token = SUIteratorNext(iterator))) {
        switch (SUTokenGetType(token)) {
            case SUTokenTypeStartFunctionDefinition: {
                SUFunction * function = SUFunctionCreate(functions, iterator, errors);
                if (function) {
                    SUListAddValue(functions, function);
                    SURelease(function);
                }
            } break;
                
            case SUTokenTypeWord: {
                SUStatement * statement = SUStatementCreate(functions, variables, iterator, token, errors);
                if (statement) {
                    SUListAddValue(statements, statement);
                    SURelease(statement);
                }
            } break;
                
            default: {
                // TODO: error
            } break;
        }
    }
    SURelease(variables);
    
    SUProgram * program = malloc(sizeof(SUProgram));
    SUInitialize(program, NULL, NULL, suess_program_free);
    program->functions = functions;
    program->statements = statements;
    
    return program;
}
