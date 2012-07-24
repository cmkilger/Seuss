//
//  SURange.h
//  suess
//
//  Created by Cory Kilger on 7/15/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SURange_h
#define suess_SURange_h

#include <stdlib.h>

typedef struct suess_range {
    size_t location;
    size_t length;
} SURange;

static inline SURange SURangeMake(size_t loc, size_t len) {
    SURange r;
    r.location = loc;
    r.length = len;
    return r;
}

static inline size_t SURangeMax(SURange range) {
    return (range.location + range.length);
}

static inline int SURangeContainsLocation(SURange range, size_t loc) {
    return (loc - range.location < range.length);
}

static inline int SURangeEqual(SURange range1, SURange range2) {
    return (range1.location == range2.location && range1.length == range2.length);
}

#endif
