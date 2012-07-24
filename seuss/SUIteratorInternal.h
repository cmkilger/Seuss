//
//  SUIteratorInternal.h
//  seuss
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUIteratorInternal_h
#define seuss_SUIteratorInternal_h

#include "SUIterator.h"
#include "SUTypeInternal.h"

struct seuss_list;
struct seuss_list_node;

struct seuss_iterator {
    SUType __base;
    struct seuss_list_node * next;
};

SUIterator * SUIteratorCreateWithList(struct seuss_list * list);

#endif
