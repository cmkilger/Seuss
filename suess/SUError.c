//
//  SUError.c
//  suess
//
//  Created by Cory Kilger on 7/18/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUError.h"
#include "SUString.h"

struct suess_error {
    SUErrorType type;
    SUString * file;
    unsigned int line;
    SUString * message;
};

SUError * SUErrorCreate(SUErrorType type, struct suess_string * file, unsigned int line, struct suess_string * message) {
    return NULL;
}