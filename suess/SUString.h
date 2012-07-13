//
//  SUString.h
//  suess
//
//  Created by Cory Kilger on 7/12/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUString_h
#define suess_SUString_h

#include <stdlib.h>

typedef struct suess_string SUString;

SUString * SUStringCreate(const char * cString);
size_t SUStringGetLength(SUString * string);

#endif
