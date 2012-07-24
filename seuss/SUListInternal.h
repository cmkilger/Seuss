//
//  SUListInternal.h
//  seuss
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUListInternal_h
#define seuss_SUListInternal_h

#include "SUList.h"
#include "SUTypeInternal.h"

typedef struct seuss_list_node {
    SUType __base;
    void * value;
    struct seuss_list_node * next;
} SUListNode;

struct seuss_list {
    SUType __base;
    struct seuss_list_node * head;
    struct seuss_list_node * tail;
    unsigned int length;
};

#endif
