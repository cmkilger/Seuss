//
//  SUError.h
//  suess
//
//  Created by Cory Kilger on 7/18/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUError_h
#define suess_SUError_h

typedef enum {
    SUErrorTypeWarning,
    SUErrorTypeError,
} SUErrorType;

struct suess_string;

typedef struct suess_error SUError;

SUError * SUErrorCreate(SUErrorType type, struct suess_string * file, unsigned int line, struct suess_string * message);
SUErrorType SUErrorGetType(SUError * error);
struct suess_string * SUErrorGetFile(SUError * error);
unsigned int SUErrorGetLine(SUError * error);
struct suess_string * SUErrorGetMessage(SUError * error);

#endif
