//
//  SUListInternal.h
//  suess
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUListInternal_h
#define suess_SUListInternal_h

#include "SUList.h"
#include "SUTypeInternal.h"

typedef struct suess_list_node {
    SUType __base;
    void * value;
    struct suess_list_node * next;
} SUListNode;

struct suess_list {
    SUType __base;
    struct suess_list_node * head;
    struct suess_list_node * tail;
    unsigned int length;
};

#endif
