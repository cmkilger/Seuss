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

typedef enum {
    SUFunctionWordTypeSignature,
    SUFunctionWordTypeParameter,
} SUFunctionWordType;

struct suess_function {
    SUType __base;
    SUList * declaration;
    SUList * statements;
};

typedef struct suess_function_word {
    SUType __base;
    SUFunctionWordType type;
    SUString * value;
} SUFunctionWord;

void suess_function_word_free(SUTypeRef type) {
    SUFunctionWord * functionWord = type;
    SURelease(functionWord->value);
    suess_free(type);
}

void suess_function_free(SUTypeRef type) {
    SUFunction * function = type;
    if (function->declaration)
        SURelease(function->declaration);
    if (function->statements)
        SURelease(function->statements);
    suess_free(type);
}

int SUFunctionMatchesDeclaration(SUFunction * function, SUList * declaration) {
    SUIterator * iterator1 = SUListCreateIterator(function->declaration);
    SUIterator * iterator2 = SUListCreateIterator(declaration);
    
    SUFunctionWord * word1 = NULL;
    SUFunctionWord * word2 = NULL;
    
    while ((word1 = SUIteratorNext(iterator1)) && (word2 = SUIteratorNext(iterator2))) {
        if (word1->type != word2->type)
            return 0;
        while (word1 && word1->type == SUFunctionWordTypeParameter)
            word1 = SUIteratorNext(iterator1);
        while (word2 && word2->type == SUFunctionWordTypeParameter)
            word2 = SUIteratorNext(iterator2);
        if ((word1 && !word2) || (!word1 && word2) || (word1 && word2 && word1->type != word2->type))
            return 0;
    }
    
    SURelease(iterator1);
    SURelease(iterator2);
    
    if (word1 || word2)
        return 0;
    
    return 1;
}

int SUFunctionContainsDeclaration(struct suess_list *functions, SUList *declaration) {
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

SUList * SUFunctionCreateDeclaration(SUIterator * iterator, char ** error) {
    SUList * declaration = SUListCreate();
    
    int hasWord = 0;
    int parameter = 0;
    SUToken * token = NULL;
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypeColon) {
        switch (SUTokenGetType(token)) {
            case SUTokenTypeWord: {
                SUFunctionWord * word = malloc(sizeof(SUFunctionWord));
                SUInitialize(word, NULL, NULL, suess_function_word_free);
                word->type = (parameter) ? SUFunctionWordTypeParameter : SUFunctionWordTypeSignature;
                word->value = SURetain(SUTokenGetValue(token));
                SUListAddValue(declaration, word, 1);
                SURelease(word);
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
            } break;
                
            case SUTokenTypeCloseParenthesis: {
                if (!parameter) {
                    // TODO: error
                }
                parameter = 0;
                hasWord = 0;
            } break;
                
            default: {
                // TODO: error
            } break;
        }
    }
    
    return declaration;
}

SUFunction * SUFunctionCreate(SUList * functions, SUIterator * iterator, char ** error) {
    SUList * declaration = SUFunctionCreateDeclaration(iterator, error);
    if (SUFunctionContainsDeclaration(functions, declaration)) {
        // TODO: error;
        SURelease(declaration);
        return NULL;
    }
    
    SUList * statements = SUListCreate();
    SUToken * token = NULL;
    while ((token = SUIteratorNext(iterator)) && SUTokenGetType(token) != SUTokenTypeEndFunctionDefinition) {
        SUStatement * statement = SUStatementCreate(functions, iterator, error);
        if (statement) {
            SUListAddValue(statements, statement, 1);
            SURelease(statement);
        }
    }
    
    SUFunction * function = malloc(sizeof(SUFunction));
    SUInitialize(function, NULL, NULL, suess_function_free);
    function->declaration = declaration;
    function->statements = statements;
    
    return function;
}