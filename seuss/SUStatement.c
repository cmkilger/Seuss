//
//  SUStatement.c
//  seuss
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUStatement.h"
#include "SUTokenizer.h"
#include "SUTypeInternal.h"
#include "SUIterator.h"
#include "SUFunction.h"
#include "SUVariable.h"
#include "SUProgram.h"
#include "SUString.h"
#include "SUError.h"
#include "SUList.h"
#include <stdlib.h>
#include <stdio.h>

struct seuss_statement {
    SUType __base;
    SUFunction * function;
    SUList * parameters;
};

void seuss_statement_free(SUTypeRef type) {
    SUStatement * statement = type;
    SURelease(statement->function);
    SURelease(statement->parameters);
    seuss_free(type);
}

SUStatement * SUStatementTieBreaker(SUList * statements) {
    // TODO: make this smarter
    return SUListGetValueAtIndex(statements, 0);
}

int SUStatementScoreParametersWithVariables(SUList * parameters, SUList * variables, SUList * parameterVariables) {
    int score = 0;
    SUList * parameter = NULL;
    SUIterator * parameterIterator = SUListCreateIterator(parameters);
    while ((parameter = SUIteratorNext(parameterIterator))) {
        SUToken * token = SUListGetValueAtIndex(parameter, 0);
        if (SUTokenGetType(token) == SUTokenTypeString) {
            score++;
            SUListAddValue(parameterVariables, SUTokenGetValue(token));
        }
        else {
            int foundMatch = 0;
            SUVariable * variable = NULL;
            SUIterator * variableIterator = SUListCreateIterator(variables);
            while ((variable = SUIteratorNext(variableIterator))) {
                SUIterator * parameterWordIterator = SUListCreateIterator(parameter);
                SUIterator * variableWordIterator = SUListCreateIterator(SUVariableGetName(variable));
                SUToken * parameterToken = SUIteratorNext(parameterWordIterator);
                SUToken * variableToken = SUIteratorNext(variableWordIterator);
                while (parameterToken && variableToken) {
                    if (SUTokenGetType(parameterToken) == SUTokenTypeString) {
                        // TODO: error
                        break;
                    }
                    if (!SUStringEqual(SUTokenGetValue(parameterToken), SUTokenGetValue(variableToken)))
                        break;
                    parameterToken = SUIteratorNext(parameterWordIterator);
                    variableToken = SUIteratorNext(variableWordIterator);
                }
                SURelease(parameterWordIterator);
                SURelease(variableWordIterator);
                if (!(parameterToken || variableToken)) {
                    SUListAddValue(parameterVariables, variable);
                    foundMatch = 1;
                    score++;
                }
            }
            SURelease(variableIterator);
            if (!foundMatch) {
                SUVariable * variable = SUVariableCreate(parameter);
                SUListAddValue(parameterVariables, variable);
                SURelease(variable);
            }
        }
    }
    SURelease(parameterIterator);
    return 0;
}

SUStatement * SUStatementCreate(SUList * functions, SUList * variables, SUIterator * iterator, SUToken * token, SUList * errors) {
    
    SUStatement * statement = NULL;
    
    // Parse the statement into only words, strings, and expressions
    
    SUList * statementTokens = SUListCreate();
    SUListAddValue(statementTokens, token);
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypePeriod) {
        SUListAddValue(statementTokens, token);
    }
    
    // Loop throught known functions
    // Match each function to the statement, return NULL if not a match, list of parameters if it is
    int score = 0;
    SUList * statements = SUListCreate();
    SUIterator * functionIterator = SUListCreateIterator(functions);
    SUFunction * function = NULL;
    while ((function = SUIteratorNext(functionIterator))) {
        SUList * parameterList = SUFunctionCreateParametersForStatementTokens(function, statementTokens);
        
        unsigned int length = SUListGetLength(parameterList);
        if (length > 0) {
            SUList * parameters = NULL;
            SUIterator * parameterIterator = SUListCreateIterator(parameterList);
            while ((parameters = SUIteratorNext(parameterIterator))) {
                SUList * parameterVariables = SUListCreate();
                int newScore = SUStatementScoreParametersWithVariables(parameters, variables, parameterVariables);
                if (newScore > score) {
                    SURelease(statements);
                    statements = SUListCreate();
                }
                if (newScore >= score) {
                    SUStatement * statement = malloc(sizeof(SUStatement));
                    SUInitialize(statement, NULL, NULL, seuss_statement_free);
                    statement->function = SURetain(function);
                    statement->parameters = SURetain(parameterVariables);
                    SUListAddValue(statements, statement);
                    SURelease(statement);
                }
                SURelease(parameterVariables);
            }
            SURelease(parameterIterator);
        }
        SURelease(parameterList);
    }
    SURelease(functionIterator);
    
    // Compare parameters to known variables and use function with the highest percentage of knowns
    // If there is a tie, use the function that starts with the most words
    if (SUListGetLength(statements) == 0) {
        const char * message = "This didn't match any known functions.";
        SUError * error = SUErrorCreate(SUErrorTypeError, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
        SUListAddValue(errors, error);
        SURelease(error);
    }
    else {
        statement = SURetain(SUStatementTieBreaker(statements));
        
        // Add new variables to known list
        SUType * variable = NULL;
        SUIterator * variableIterator = SUListCreateIterator(statement->parameters);
        while ((variable = SUIteratorNext(variableIterator))) {
            if (SUTypeIsVariable(variable) && !SUListContainsValue(variables, variable))
                SUListAddValue(variables, variable);
        }
        SURelease(variableIterator);
    }
    
    SURelease(statementTokens);
    SURelease(statements);
    
    return statement;
}

SUFunction * SUStatementGetFunction(SUStatement * statement) {
    return statement->function;
}

SUList * SUStatementGetParameters(SUStatement * statement) {
    return statement->parameters;
}

void SUStatementExecute(SUProgram * program, SUStatement * statement) {
    SUFunctionExecute(program, statement->function, statement->parameters);
}
