//
//  SUError.h
//  seuss
//
//  Created by Cory Kilger on 7/18/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUError_h
#define seuss_SUError_h

typedef enum {
    SUErrorTypeWarning,
    SUErrorTypeError,
} SUErrorType;

struct seuss_string;

typedef struct seuss_error SUError;

SUError * SUErrorCreate(SUErrorType type, struct seuss_string * file, unsigned int line, struct seuss_string * message);
SUErrorType SUErrorGetType(SUError * error);
struct seuss_string * SUErrorGetFile(SUError * error);
unsigned int SUErrorGetLine(SUError * error);
struct seuss_string * SUErrorGetMessage(SUError * error);

#endif
