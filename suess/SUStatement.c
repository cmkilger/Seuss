//
//  SUStatement.c
//  suess
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUStatement.h"
#include "SUTokenizer.h"
#include "SUTypeInternal.h"
#include "SUIterator.h"
#include "SUFunction.h"
#include "SUString.h"
#include "SUList.h"
#include <stdlib.h>
#include <stdio.h>

struct suess_statement {
    SUType __base;
    SUFunction * function;
    SUList * parameters;
};

void suess_statement_free(SUTypeRef type) {
    SUStatement * statement = type;
    SURelease(statement->function);
    SURelease(statement->parameters);
    suess_free(type);
}

SUStatement * SUStatementTieBreaker(SUList * statements) {
    return SUListGetValueAtIndex(statements, 0);
}

int SUStatementScoreParametersWithVariables(SUList * parameters, SUList * variables) {
    int score = 0;
    SUList * parameter = NULL;
    SUIterator * parameterIterator = SUListCreateIterator(parameters);
    while ((parameter = SUIteratorNext(parameterIterator))) {
        SUList * variable = NULL;
        SUIterator * variableIterator = SUListCreateIterator(variables);
        while ((variable = SUIteratorNext(variableIterator))) {
            SUString * parameterWord = NULL;
            SUString * variableWord = NULL;
            SUIterator * parameterWordIterator = SUListCreateIterator(parameter);
            SUIterator * variableWordIterator = SUListCreateIterator(variable);
            while ((parameterWord = SUIteratorNext(parameterWordIterator)) && (variableWord = SUIteratorNext(variableWordIterator))) {
                if (!SUStringEqual(parameterWord, variableWord))
                    break;
            }
            SURelease(parameterWordIterator);
            SURelease(variableWordIterator);
            if (!(parameterWord || variableWord))
                score++;
        }
        SURelease(variableIterator);
    }
    SURelease(parameterIterator);
    return 0;
}

SUStatement * SUStatementCreate(SUList * functions, SUList * variables, SUIterator * iterator, SUToken * token, char ** error) {
    
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
                int newScore = SUStatementScoreParametersWithVariables(parameters, variables);
                if (newScore > score) {
                    SURelease(statements);
                    statements = SUListCreate();
                }
                if (newScore >= score) {
                    SUStatement * statement = malloc(sizeof(SUStatement));
                    SUInitialize(statement, NULL, NULL, suess_statement_free);
                    statement->function = SURetain(function);
                    statement->parameters = SURetain(parameters);
                    SUListAddValue(statements, statement);
                    SURelease(statement);
                }
            }
            SURelease(parameterIterator);
        }
        SURelease(parameterList);
    }
    SURelease(functionIterator);
    
    // Compare parameters to known variables and use function with the highest percentage of knowns
    // If there is a tie, use the function that starts with the most words
    if (SUListGetLength(statements) == 0) {
        // TODO: error
    }
    else {
        statement = SURetain(SUStatementTieBreaker(statements));
        
        // Add new variables to known list
        SUList * parameter = NULL;
        SUIterator * parameterIterator = SUListCreateIterator(statement->parameters);
        while ((parameter = SUIteratorNext(parameterIterator)))
            SUListAddValue(variables, parameter);
        SURelease(parameterIterator);
    }
    
    SURelease(statementTokens);
    SURelease(statements);
    
    return statement;
}
