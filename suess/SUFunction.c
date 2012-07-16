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

SUList * SUFunctionCreateDeclaration(SUIterator * iterator, SUList ** parametersPtr, char ** error) {
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
                    // TODO: error
                }
                if (parameter) {
                    // TODO: error
                }
                parameter = 1;
                currentList = NULL;
            } break;
                
            case SUTokenTypeCloseParenthesis: {
                if (!parameter) {
                    // TODO: error
                }
                parameter = 0;
                hasWord = 0;
                currentList = NULL;
            } break;
                
            default: {
                // TODO: error
            } break;
        }
    }
    
    *parametersPtr = parameters;
    
    return signature;
}

SUFunction * SUFunctionCreate(SUList * functions, SUIterator * iterator, char ** error) {
    SUList * parameters = NULL;
    SUList * signature = SUFunctionCreateDeclaration(iterator, &parameters, error);
    if (SUFunctionContainsDeclaration(functions, signature)) {
        // TODO: error;
        SURelease(parameters);
        SURelease(signature);
        return NULL;
    }
    
    SUList * variables = SUListCreate();
    SUList * statements = SUListCreate();
    SUToken * token = NULL;
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypeEndFunctionDefinition) {
        SUStatement * statement = SUStatementCreate(functions, variables, iterator, token, error);
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

SUList * SUFunctionCreateListOfTokensThatMatchWords(SUList * statement, int startIndex, SUList * words) {
    SUList * tokens = SUListCreate();
    
    SUToken * startToken = NULL;
    SUIterator * startIterator = SUListCreateIterator(statement);
    for (int i = 0; i <= startIndex; i++)
        startToken = SUIteratorNext(startIterator);
    
    while (startToken) {
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
        
        startToken = SUIteratorNext(startIterator);
        
        SURelease(wordIterator);
        SURelease(tokenIterator);
        
        if (startIndex == 0)
            break;
    }
    
    SURelease(startIterator);
    
    return tokens;
}

SUList * SUFunctionCreateListOfParameters(SUList * statement, int startIndex, SUFunction * function, size_t signatureIndex) {
    // If there are no more signature words, just take the rest of the stamement as a parameter
    if (signatureIndex >= SUListGetLength(function->signature)) {
        SURange range = SURangeMake(startIndex, SUListGetLength(statement)-startIndex);
        SUList * words = SUListCreateSublistWithRange(statement, range);
        SUList * parameters = SUListCreate();
        SUListAddValue(parameters, words);
        SUList * options = SUListCreate();
        SUListAddValue(options, parameters);
        SURelease(parameters);
        SURelease(words);
        return options;
    }
    
    // Get the next words
    SUList * nextWords = NULL;
    SUIterator * nextWordsIterator = SUListCreateIterator(function->signature);
    for (int i = 0; i <= signatureIndex; i++)
        nextWords = SUIteratorNext(nextWordsIterator);
    SURelease(nextWordsIterator);
    
    // Create result options array
    SUList * options = SUListCreate();
    
    // Enumerate matches for nextToken in statement
    SUList * matches = SUFunctionCreateListOfTokensThatMatchWords(statement, startIndex, nextWords);
    if (SUListGetLength(matches) > 0) {
        SUIterator * matchIterator = SUListCreateIterator(matches);
        SUToken * match = NULL;
        while ((match = SUIteratorNext(matchIterator))) {
            size_t location = SUListIndexOfValue(statement, match);
            SUList * parameter = SUListCreateSublistWithRange(statement, SURangeMake(startIndex, location - startIndex));
            if (SUListGetLength(parameter) > 0 || startIndex == 0) {
                SUList * parameters = SUFunctionCreateListOfParameters(statement, location + SUListGetLength(nextWords), function, signatureIndex+1);
                SUIterator * parameterIterator = SUListCreateIterator(parameters);
                SUList * array = NULL;
                while ((array = SUIteratorNext(parameterIterator))) {
                    if (startIndex > 0)
                        SUListInsertValue(array, parameter, 0);
                    SUListAddValue(options, array);
                }
                SURelease(parameterIterator);
                SURelease(parameters);
            }
            SURelease(parameter);
        }
        SURelease(matchIterator);
    }
    SURelease(matches);
    
    return options;
}

SUList * SUFunctionCreateParametersForStatementTokens(SUFunction * function, SUList * tokens) {
    return SUFunctionCreateListOfParameters(tokens, 0, function, 0);
}
