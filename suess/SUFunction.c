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

typedef enum {
	SUFunctionWordTypeSignature,
	SUFunctionWordTypeParameter,
} SUFunctionWordType;

struct suess_function {
	SUType __base;
	SUList * declaration;
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
	SURelease(function->declaration);
	suess_free(type);
}

SUFunction * SUFunctionCreate(SUIterator * iterator, char ** error) {
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
	
	SUFunction * function = malloc(sizeof(SUFunction));
	SUInitialize(function, NULL, NULL, suess_function_free);
	function->declaration = declaration;
	
	return function;
}