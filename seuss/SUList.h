//
//  SUList.h
//  seuss
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUList_h
#define seuss_SUList_h

#include "SUType.h"
#include "SURange.h"
#include <stdlib.h>

struct seuss_iterator;

typedef struct seuss_list SUList;

SUList * SUListCreate();
void SUListAddValue(SUList * list, SUTypeRef value);
unsigned int SUListGetLength(SUList * list);
SUTypeRef SUListGetValueAtIndex(SUList * list, unsigned int index);
unsigned int SUListIndexOfValue(SUList * list, SUTypeRef value);
int SUListContainsValue(SUList * list, SUTypeRef);
void SUListInsertValue(SUList * list, SUTypeRef value, unsigned int index);
SUList * SUListCreateSublistWithRange(SUList * list, SURange range);
struct seuss_iterator * SUListCreateIterator(SUList * list);

#endif
