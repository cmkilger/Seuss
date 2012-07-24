//
//  SUList.h
//  suess
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUList_h
#define suess_SUList_h

#include "SUType.h"
#include "SURange.h"
#include <stdlib.h>

struct suess_iterator;

typedef struct suess_list SUList;

SUList * SUListCreate();
void SUListAddValue(SUList * list, SUTypeRef value);
unsigned int SUListGetLength(SUList * list);
SUTypeRef SUListGetValueAtIndex(SUList * list, unsigned int index);
unsigned int SUListIndexOfValue(SUList * list, SUTypeRef value);
int SUListContainsValue(SUList * list, SUTypeRef);
void SUListInsertValue(SUList * list, SUTypeRef value, unsigned int index);
SUList * SUListCreateSublistWithRange(SUList * list, SURange range);
struct suess_iterator * SUListCreateIterator(SUList * list);

#endif
