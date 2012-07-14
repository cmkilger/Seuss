//
//  SUSyntaxTree.h
//  suess
//
//  Created by Cory Kilger on 7/9/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUSyntaxTree_h
#define suess_SUSyntaxTree_h

#include "SUTokenizer.h"
#include "SUList.h"

typedef struct suess_syntaxTree {
    struct suess_program * program;
} SUSyntaxTree;

SUSyntaxTree * SUSyntaxTreeCreate(SUList * tokens);

#endif
