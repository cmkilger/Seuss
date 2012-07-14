//
//  SUList.c
//  suess
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUListInternal.h"
#include "SUIteratorInternal.h"
#include "SUTypeInternal.h"
#include <stdlib.h>

void suess_list_free(SUTypeRef type) {
    SUList * list = type;
    if (list->head)
        SURelease(list->head);
    suess_free(type);
}

SUList * SUListCreate() {
    SUList * list = malloc(sizeof(SUList));
    SUInitialize(list, NULL, NULL, suess_list_free);
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void suess_list_node_free(SUTypeRef type) {
    SUListNode * node = type;
    if (node->next)
        SURelease(node->next);
    if (node->shouldRetain)
        SURelease(node->value);
    suess_free(type);
}

void SUListAddValue(SUList * list, void * value, int shouldRetain) {
    SUListNode * new = malloc(sizeof(SUListNode));
    SUInitialize(new, NULL, NULL, suess_list_node_free);
    new->value = value;
    new->next = NULL;
    if ((new->shouldRetain = shouldRetain))
        SURetain(value);
    SUListNode * node = list->tail;
    if (node) {
        node->next = new;
        list->tail = new;
    }
    else {
        list->head = new;
        list->tail = new;
    }
}

SUIterator * SUListCreateIterator(SUList * list) {
    return SUIteratorCreateWithList(list);
}