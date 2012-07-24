//
//  SUList.c
//  seuss
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUListInternal.h"
#include "SUIteratorInternal.h"
#include "SUTypeInternal.h"
#include <stdlib.h>
#include <assert.h>

void seuss_list_free(SUTypeRef type) {
    SUList * list = type;
    if (list->head)
        SURelease(list->head);
    seuss_free(type);
}

SUList * SUListCreate() {
    SUList * list = malloc(sizeof(SUList));
    SUInitialize(list, NULL, NULL, seuss_list_free);
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void seuss_list_node_free(SUTypeRef type) {
    SUListNode * node = type;
    if (node->next)
        SURelease(node->next);
    SURelease(node->value);
    seuss_free(type);
}

void SUListAddValue(SUList * list, void * value) {
    assert(value != NULL);
    
    SUListNode * new = malloc(sizeof(SUListNode));
    SUInitialize(new, NULL, NULL, seuss_list_node_free);
    new->value = SURetain(value);
    new->next = NULL;
    SUListNode * node = list->tail;
    if (node) {
        node->next = new;
        list->tail = new;
    }
    else {
        list->head = new;
        list->tail = new;
    }
    
    list->length++;
    
    if (list->length > 1000) {
        SURelease(SUListCreate());
    }
}

unsigned int SUListGetLength(SUList * list) {
    return list->length;
}

SUTypeRef SUListGetValueAtIndex(SUList * list, unsigned int index) {
    assert(index < list->length);
    SUListNode * node = list->head;
    for (unsigned int i = 0; i < index; i++)
        node = node->next;
    return node->value;
}

unsigned int SUListIndexOfValue(SUList * list, SUTypeRef value) {
    SUListNode * node = list->head;
    unsigned int index = 0;
    while (node->value != value) {
        index++;
        if (node->next)
            node = node->next;
        else
            break;
    }
    return index;
}

int SUListContainsValue(SUList * list, SUTypeRef value) {
    SUListNode * node = list->head;
    while (node) {
        if (node->value == value)
            return 1;
        node = node->next;
    }
    return 0;
}

void SUListInsertValue(SUList * list, SUTypeRef value, unsigned int index) {
    assert(index <= list->length);
    
    SUListNode * new = malloc(sizeof(SUListNode));
    SUInitialize(new, NULL, NULL, seuss_list_node_free);
    new->value = SURetain(value);
    
    if (index == 0) {
        new->next = list->head;
        list->head = new;
        if (!list->tail)
            list->tail = new;
    }
    else {
        SUListNode * node = list->head;
        for (unsigned int i = 0; i < index-1; i++)
            node = node->next;
        new->next = node->next;
        node->next = new;
        if (node == list->tail)
            list->tail = new;
    }
    
    list->length++;
    
    if (list->length > 1000) {
        SURelease(SUListCreate());
    }
}

SUList * SUListCreateSublistWithRange(SUList * list, SURange range) {
    assert(SURangeMax(range) <= list->length);
    
    SUList * sublist = SUListCreate();
    
    SUListNode * node = list->head;
    for (unsigned int i = 0; i < range.location; i++)
        node = node->next;
    
    for (unsigned int i = 0; i < range.length; i++) {
        SUListAddValue(sublist, node->value);
        node = node->next;
    }
    
    return sublist;
}

SUIterator * SUListCreateIterator(SUList * list) {
    return SUIteratorCreateWithList(list);
}