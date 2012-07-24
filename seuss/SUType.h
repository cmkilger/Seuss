//
//  SUType.h
//  seuss
//
//  Created by Cory Kilger on 7/10/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUType_h
#define seuss_SUType_h

typedef void * SUTypeRef;

SUTypeRef SURetain(SUTypeRef type);
void SURelease(SUTypeRef type);

#endif
