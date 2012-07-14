//
//  SUProgram.h
//  suess
//
//  Created by Cory Kilger on 7/13/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUProgram_h
#define suess_SUProgram_h

struct suess_iterator;

typedef struct suess_program SUProgram;

SUProgram * SUProgramCreate(struct suess_iterator * iterator, char ** error);

#endif
