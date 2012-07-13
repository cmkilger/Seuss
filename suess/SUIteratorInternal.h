//
//  SUIteratorInternal.h
//  suess
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUIteratorInternal_h
#define suess_SUIteratorInternal_h

#include "SUIterator.h"
#include "SUTypeInternal.h"

struct suess_list;
struct suess_list_node;

struct suess_iterator {
	SUType __base;
	struct suess_list_node * next;
};

SUIterator * SUIteratorCreateWithList(struct suess_list * list);

#endif
