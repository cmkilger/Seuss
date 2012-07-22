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
#include "SUVariable.h"
#include "SUString.h"
#include "SUError.h"

struct suess_program {
    SUType __base;
    SUList * functions;
    SUList * statements;
    void * writeData;
    SUProgramWriteCallback writeCallback;
    void * readData;
    SUProgramReadCallback readCallback;
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
    
    SUList * functions = SUFunctionCreateBuiltins();
    SUList * statements = SUListCreate();
    SUList * variables = SUVariableCreateBuiltins();
    
    SUToken * token = NULL;
    while ((token = SUIteratorNext(iterator))) {
        switch (SUTokenGetType(token)) {
            case SUTokenTypeStartFunctionDefinition: {
                SUFunction * function = SUFunctionCreate(functions, iterator, token, errors);
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
                const char * message = "What have you done!?";
                SUError * error = SUErrorCreate(SUErrorTypeError, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
                SUListAddValue(errors, error);
                SURelease(error);
            } break;
        }
    }
    SURelease(variables);
    SURelease(iterator);
    
    SUProgram * program = malloc(sizeof(SUProgram));
    SUInitialize(program, NULL, NULL, suess_program_free);
    program->functions = functions;
    program->statements = statements;
    
    return program;
}

void SUProgramExecute(SUProgram * program) {
    SUStatement * statement = NULL;
    SUIterator * statementIterator = SUListCreateIterator(program->statements);
    while ((statement = SUIteratorNext(statementIterator)))
        SUStatementExecute(program, statement);
    SURelease(statementIterator);
}

SUProgramWriteCallback SUProgramGetWriteCallback(SUProgram * program) {
    return program->writeCallback;
}

void SUProgramSetWriteCallback(SUProgram * program, SUProgramWriteCallback writeCallback, void * userData) {
    program->writeCallback = writeCallback;
    program->writeData = userData;
}

SUProgramReadCallback SUProgramGetReadCallback(SUProgram * program) {
    return program->readCallback;
}

void SUProgramSetReadCallback(SUProgram * program, SUProgramReadCallback readCallback, void * userData) {
    program->readCallback = readCallback;
    program->readData = userData;
}

void * SUProgramGetWriteData(SUProgram * program) {
    return program->writeData;
}

void * SUProgramGetReadData(SUProgram * program) {
    return program->readData;
}
