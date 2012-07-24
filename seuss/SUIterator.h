//
//  SUIterator.h
//  seuss
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUIterator_h
#define seuss_SUIterator_h

#include "SUType.h"

typedef struct seuss_iterator SUIterator;

SUTypeRef SUIteratorNext(SUIterator * iterator);

#endif
