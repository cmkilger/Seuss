//
//  SUString.c
//  suess
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUString.h"
#include "SUTypeInternal.h"
#include <string.h>

struct suess_string {
    SUType __base;
    const char * string;
    size_t length;
};

void suess_string_free(SUTypeRef type) {
    SUString * string = type;
    if (string->length > 0)
        free((char *)string->string);
    suess_free(type);
}

SUString * SUStringCreate(const char * cString) {
    SUString * string = malloc(sizeof(SUString));
    SUInitialize(string, NULL, NULL, suess_string_free);
    size_t length = 0;
    if (cString && (length = strlen(cString)) > 0) {
        char * buffer = malloc(sizeof(char)*(length+1));
        strcpy(buffer, cString);
        buffer[length] = 0;
        string->string = buffer;
        string->length = length;
    }
    else {
        string->string = "";
        string->length = 0;
    }
    return string;
}

size_t SUStringGetLength(SUString * string) {
    return string->length;
}