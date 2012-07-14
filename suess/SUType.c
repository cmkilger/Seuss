//
//  SUType.c
//  suess
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUType.h"
#include "SUTypeInternal.h"

void SUInitialize(SUTypeRef typeRef, SUTypeRef(*retain)(SUTypeRef), void(*release)(SUTypeRef), void(*free)(SUTypeRef)) {
    SUType * type = (SUType *)typeRef;
    type->retainCount = 1;
    
    if (retain)
        type->retain = retain;
    else
        type->retain = suess_retain;
    
    if (release)
        type->release = release;
    else
        type->release = suess_release;
    
    if (free)
        type->free = free;
    else
        type->free = suess_free;
}

SUTypeRef SURetain(SUTypeRef type) {
    return ((SUType *)type)->retain(type);
}

void SURelease(SUTypeRef type) {
    ((SUType *)type)->release(type);
}

void SUFree(SUTypeRef type) {
    ((SUType *)type)->free(type);
}
