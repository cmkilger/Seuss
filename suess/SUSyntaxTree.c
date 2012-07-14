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
#include "SUStatement.h"
#include "SUProgram.h"
#include <stdlib.h>

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