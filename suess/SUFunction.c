//
//  SUFunction.c
//  suess
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUFunction.h"
#include "SUTypeInternal.h"
#include "SUList.h"
#include "SUString.h"
#include "SUIterator.h"
#include "SUTokenizer.h"
#include "SUStatement.h"
#include "SUError.h"
#include "SURange.h"

typedef enum {
    SUFunctionWordTypeSignature,
    SUFunctionWordTypeParameter,
} SUFunctionWordType;

struct suess_function {
    SUType __base;
    SUList * signature;
    SUList * parameters;
    SUList * statements;
};

void suess_function_free(SUTypeRef type) {
    SUFunction * function = type;
    if (function->signature)
        SURelease(function->signature);
    if (function->parameters)
        SURelease(function->parameters);
    if (function->statements)
        SURelease(function->statements);
    suess_free(type);
}

int SUFunctionMatchesDeclaration(SUFunction * function, SUList * declaration) {
    SUIterator * signatureIterator1 = SUListCreateIterator(function->signature);
    SUIterator * signatureIterator2 = SUListCreateIterator(declaration);
    
    SUList * list1 = NULL;
    SUList * list2 = NULL;
    
    while ((list1 = SUIteratorNext(signatureIterator1)) && (list2 = SUIteratorNext(signatureIterator2))) {
        SUIterator * listIterator1 = SUListCreateIterator(list1);
        SUIterator * listIterator2 = SUListCreateIterator(list2);
        
        SUString * word1 = NULL;
        SUString * word2 = NULL;
        
        while ((word1 = SUIteratorNext(listIterator1)) && (word2 = SUIteratorNext(listIterator2))) {
            if (!SUStringEqual(word1, word2)) {
                SURelease(signatureIterator1);
                SURelease(signatureIterator2);
                SURelease(listIterator1);
                SURelease(listIterator2);
                return 0;
            }
        }
        
        if ((word1 && !word2) || (!word1 && word2)) {
            SURelease(signatureIterator1);
            SURelease(signatureIterator2);
            SURelease(listIterator1);
            SURelease(listIterator2);
            return 0;
        }
        
        SURelease(listIterator1);
        SURelease(listIterator2);
    }
    
    SURelease(signatureIterator1);
    SURelease(signatureIterator2);
    
    if (list1 || list2)
        return 0;
    
    return 1;
}

int SUFunctionContainsDeclaration(SUList * functions, SUList * declaration) {
    SUFunction * oldFunction = NULL;
    SUIterator * oldFunctionIterator = SUListCreateIterator(functions);
    while ((oldFunction = SUIteratorNext(oldFunctionIterator))) {
        int matches = SUFunctionMatchesDeclaration(oldFunction, declaration);
        if (matches) {
            SURelease(oldFunctionIterator);
            return 1;
        }
    }
    SURelease(oldFunctionIterator);
    
    return 0;
}

SUList * SUFunctionCreateDeclaration(SUIterator * iterator, SUList ** parametersPtr, SUList * errors) {
    SUList * signature = SUListCreate();
    SUList * parameters = SUListCreate();
    
    int hasWord = 0;
    int parameter = 0;
    SUToken * token = NULL;
    SUList * currentList = NULL;
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypeColon) {
        switch (SUTokenGetType(token)) {
            case SUTokenTypeWord: {
                if (!currentList) {
                    currentList = SUListCreate();
                    if (parameter)
                        SUListAddValue(parameters, currentList);
                    else
                        SUListAddValue(signature, currentList);
                    SURelease(currentList);
                }
                SUListAddValue(currentList, SUTokenGetValue(token));
                hasWord = 1;
            } break;
                
            case SUTokenTypeOpenParenthesis: {
                if (!hasWord) {
                    const char * message = "Functions need words before each parameter.";
                    SUError * error = SUErrorCreate(SUErrorTypeError, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
                    SUListAddValue(errors, error);
                    SURelease(error);
                }
                if (parameter) {
                    const char * message = "Don't use '(' inside the parameter.";
                    SUError * error = SUErrorCreate(SUErrorTypeError, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
                    SUListAddValue(errors, error);
                    SURelease(error);
                }
                parameter = 1;
                currentList = NULL;
            } break;
                
            case SUTokenTypeCloseParenthesis: {
                if (!parameter) {
                    const char * message = "Only use ')' to end a parameter.";
                    SUError * error = SUErrorCreate(SUErrorTypeError, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
                    SUListAddValue(errors, error);
                    SURelease(error);
                }
                parameter = 0;
                hasWord = 0;
                currentList = NULL;
            } break;
                
            default: {
                const char * message = "What have you done!?";
                SUError * error = SUErrorCreate(SUErrorTypeError, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
                SUListAddValue(errors, error);
                SURelease(error);
            } break;
        }
    }
    
    if (hasWord || parameter) {
        const char * message = "You didn't properly close the final parameter with a parenthesis.";
        SUError * error = SUErrorCreate(SUErrorTypeWarning, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
        SUListAddValue(errors, error);
        SURelease(error);
    }
    
    *parametersPtr = parameters;
    
    return signature;
}

SUFunction * SUFunctionCreate(SUList * functions, SUIterator * iterator, SUToken * token, SUList * errors) {
    SUList * parameters = NULL;
    SUList * signature = SUFunctionCreateDeclaration(iterator, &parameters, errors);
    if (SUFunctionContainsDeclaration(functions, signature)) {
        const char * message = "You can't decare the same function twice. We're going to ignore this one.";
        SUError * error = SUErrorCreate(SUErrorTypeWarning, SUTokenGetFile(token), SUTokenGetLine(token), SUStringCreate(message));
        SUListAddValue(errors, error);
        SURelease(error);
        SURelease(parameters);
        SURelease(signature);
        return NULL;
    }
    
    SUList * variables = SUListCreate();
    SUList * statements = SUListCreate();
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypeEndFunctionDefinition) {
        SUStatement * statement = SUStatementCreate(functions, variables, iterator, token, errors);
        if (statement) {
            SUListAddValue(statements, statement);
            SURelease(statement);
        }
    }
    SURelease(variables);
    
    SUFunction * function = malloc(sizeof(SUFunction));
    SUInitialize(function, NULL, NULL, suess_function_free);
    function->signature = signature;
    function->parameters = parameters;
    function->statements = statements;
    
    return function;
}

SUList * SUFunctionCreateListOfTokensThatMatchWords(SUList * statement, int isStart, SUList * words) {
    SUList * tokens = SUListCreate();
    
    SUIterator * startIterator = SUListCreateIterator(statement);
    SUToken * startToken = NULL;
    while ((startToken = SUIteratorNext(startIterator))) {
        SUIterator * wordIterator = SUListCreateIterator(words);
        SUIterator * tokenIterator = SUListCreateIterator(statement);
        
        SUString * word = SUIteratorNext(wordIterator);
        SUToken * token = NULL;
        while (token != startToken)
            token = SUIteratorNext(tokenIterator);
        
        while (word && token) {
            if (!SUStringEqual(word, SUTokenGetValue(token)))
                break;
            word = SUIteratorNext(wordIterator);
            token = SUIteratorNext(tokenIterator);
        }
        
        if (!word)
            SUListAddValue(tokens, startToken);
        
        SURelease(wordIterator);
        SURelease(tokenIterator);
        
        if (isStart)
            break;
    }
    
    SURelease(startIterator);
    
    return tokens;
}

SUList * SUFunctionCreateListOfParameters(SUList * statement, int isStart, SUFunction * function, unsigned int signatureIndex) {
    // If there are no more signature words, just take the rest of the stamement as a parameter
    if (signatureIndex >= SUListGetLength(function->signature)) {
        SUList * parameters = SUListCreate();
        SUListAddValue(parameters, statement);
        SUList * options = SUListCreate();
        SUListAddValue(options, parameters);
        SURelease(parameters);
        return options;
    }
    
    // Get the next words
    SUList * nextWords = SUListGetValueAtIndex(function->signature, signatureIndex);
    
    // Create result options array
    SUList * options = SUListCreate();
    
    // Enumerate matches for nextToken in statement
    SUList * matches = SUFunctionCreateListOfTokensThatMatchWords(statement, isStart, nextWords);
    if (SUListGetLength(matches) > 0) {
        SUIterator * matchIterator = SUListCreateIterator(matches);
        SUToken * match = NULL;
        while ((match = SUIteratorNext(matchIterator))) {
            size_t location = SUListIndexOfValue(statement, match);
            SUList * parameter = SUListCreateSublistWithRange(statement, SURangeMake(0, location));
            if (SUListGetLength(parameter) > 0 || isStart) {
                unsigned int rangeLocation = location + SUListGetLength(nextWords);
                SURange range = SURangeMake(rangeLocation, SUListGetLength(statement) - rangeLocation);
                SUList * newStatement = SUListCreateSublistWithRange(statement, range);
                SUList * parameters = SUFunctionCreateListOfParameters(newStatement, 0, function, signatureIndex+1);
                SUIterator * parameterIterator = SUListCreateIterator(parameters);
                SUList * array = NULL;
                while ((array = SUIteratorNext(parameterIterator))) {
                    if (!isStart)
                        SUListInsertValue(array, parameter, 0);
                    if (!isStart || SUListGetLength(array) == SUListGetLength(function->parameters))
                        SUListAddValue(options, array);
                }
                SURelease(parameterIterator);
                SURelease(parameters);
                SURelease(newStatement);
            }
            SURelease(parameter);
        }
        SURelease(matchIterator);
    }
    SURelease(matches);
    
    return options;
}

SUList * SUFunctionCreateParametersForStatementTokens(SUFunction * function, SUList * tokens) {
    return SUFunctionCreateListOfParameters(tokens, 1, function, 0);
}
