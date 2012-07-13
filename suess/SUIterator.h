//
//  SUIterator.h
//  suess
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUIterator_h
#define suess_SUIterator_h

#include "SUType.h"

typedef struct suess_iterator SUIterator;

SUTypeRef SUIteratorNext(SUIterator * iterator);

#endif
