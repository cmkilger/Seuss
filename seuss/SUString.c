//
//  SUString.c
//  seuss
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUString.h"
#include "SUTypeInternal.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>

struct seuss_string {
    SUType __base;
    const char * string;
    size_t length;
};

void seuss_string_free(SUTypeRef type) {
    SUString * string = type;
    if (string->length > 0)
        free((char *)string->string);
    seuss_free(type);
}

SUString * SUStringCreate(const char * cString) {
    SUString * string = malloc(sizeof(SUString));
    SUInitialize(string, NULL, NULL, seuss_string_free);
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

const char * SUStringGetCString(SUString * string) {
    return string->string;
}

int SUStringEqual(SUString * string1, SUString * string2) {
    assert(string1->__base.free == seuss_string_free && string2->__base.free == seuss_string_free);
    size_t length1 = SUStringGetLength(string1);
    size_t length2 = SUStringGetLength(string2);
    if (length1 != length2)
        return 0;
    const char * str1 = string1->string;
    const char * str2 = string2->string;
    for (size_t i = 0; i < length1; i++) {
        if (tolower(str1[i]) != tolower(str2[i]))
            return 0;
    }
    return 1;
}
