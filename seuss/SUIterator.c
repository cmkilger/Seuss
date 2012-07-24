//
//  SUIterator.c
//  seuss
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUIteratorInternal.h"
#include "SUListInternal.h"
#include <assert.h>

void seuss_iterator_free(SUTypeRef type) {
    SUIterator * iterator = type;
    if (iterator->next)
        SURelease(iterator->next);
    seuss_free(type);
}

SUIterator * SUIteratorCreateWithList(SUList * list) {
    assert(list != NULL);
    SUIterator * iterator = malloc(sizeof(SUIterator));
    SUInitialize(iterator, NULL, NULL, seuss_iterator_free);
    iterator->next = list->head;
    if (iterator->next)
        SURetain(iterator->next);
    return iterator;
}

SUTypeRef SUIteratorNext(SUIterator * iterator) {
    assert(iterator != NULL);
    SUListNode * next = iterator->next;
    if (next) {
        if (next->next)
            SURetain(next->next);
        SURelease(iterator->next);
        iterator->next = next->next;
        return next->value;
    }
    return NULL;
}