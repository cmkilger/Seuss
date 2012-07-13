//
//  SUList.h
//  suess
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUList_h
#define suess_SUList_h

struct suess_iterator;

typedef struct suess_list SUList;

SUList * SUListCreate();
void SUListAddValue(SUList * list, void * value, int shouldRetain);
struct suess_iterator * SUListCreateIterator(SUList * list);

#endif
