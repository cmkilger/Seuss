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
#include "SUList.h"
#include <stdlib.h>

struct suess_statement {
    SUType __base;
    SUFunction * function;
    SUList * parameters;
};

SUStatement * SUStatementCreate(SUList * functions, SUList * variables, SUIterator * iterator, SUToken * token, char ** error) {
    
    // Parse the statement into only words, strings, and expressions
    
    SUList * statementTokens = SUListCreate();
    SUListAddValue(statementTokens, token);
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypePeriod) {
        SUListAddValue(statementTokens, token);
    }
    
    // Loop throught known functions
    SUIterator * functionIterator = SUListCreateIterator(functions);
    SUFunction * function = NULL;
    while ((function = SUIteratorNext(functionIterator))) {
        SUList * parameters = SUFunctionCreateParametersForStatementTokens(function, statementTokens);
        // TODO: use parameters
        SURelease(parameters);
    }
    SURelease(functionIterator);
    
    
    // Match each function to the statement, return NULL if not a match, list of parameters if it is
    // Compare parameters to known variables and use function with the highest percentage of knowns
    // If there is a tie, use the function that starts with the most words
    // Add new variables to known list
    // Create statement object with function and parameters
    
    
    
    SURelease(statementTokens);
    
    
    
    
    
    return NULL;
}