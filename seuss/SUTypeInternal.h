//
//  SUTypeInternal.h
//  seuss
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUTypeInternal_h
#define seuss_SUTypeInternal_h

#include "SUType.h"
#include <stdlib.h>

typedef struct seuss_type {
    unsigned int retainCount;
    SUTypeRef(*retain)(SUTypeRef);
    void(*release)(SUTypeRef);
    void(*free)(SUTypeRef);
} SUType;

void SUInitialize(SUTypeRef type, SUTypeRef(*retain)(SUTypeRef), void(*release)(SUTypeRef), void(*free)(SUTypeRef));
void SUFree(SUTypeRef type);

static inline SUTypeRef seuss_retain(SUTypeRef type) {
    ((SUType*)type)->retainCount++;
    return type;
}

static inline void seuss_release(SUTypeRef type) {
    if (--((SUType*)type)->retainCount == 0)
        SUFree(type);
}

static inline void seuss_free(SUTypeRef type) {
    free((SUType*)type);
}

#endif
