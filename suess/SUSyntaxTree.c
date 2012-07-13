//
//  SUSyntaxTree.c
//  suess
//
//  Created by Cory Kilger on 7/9/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUSyntaxTree.h"
#include "SUTypeInternal.h"
#include "SUString.h"
#include "SUList.h"
#include "SUIterator.h"
#include "SUFunction.h"
#include <stdlib.h>

SUStatement * SUStatementCreate(SUIterator * iterator, char ** error) {
	return NULL;
}

SUProgram * SUProgramCreate(SUIterator * iterator, char ** error) {
	SUToken * token = NULL;
	while ((token = SUIteratorNext(iterator))) {
		switch (SUTokenGetType(token)) {
			case SUTokenTypeStartFunctionDefinition: {
				SUFunction * function = SUFunctionCreate(iterator, error);
				if (function)
					SURelease(function);
			} break;
				
			case SUTokenTypeWord: {
				SUStatement * statement = SUStatementCreate(iterator, error);
				if (statement)
					SURelease(statement);
			} break;
			
			default: {
//				token = NULL;
				if (error)
					*error = "Unable to parse!";
			} break;
		}
	}
	return NULL;
}

SUSyntaxTree * SUSyntaxTreeCreate(SUList * tokens) {
	SUIterator * iterator = SUListCreateIterator(tokens);
	char * error = NULL;
	SUProgram * program = SUProgramCreate(iterator, &error);
	if (iterator)
		SURelease(iterator);
	if (program)
		SURelease(program);
	return NULL;
}