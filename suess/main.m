//
//  main.m
//  suess
//
//  Created by Cory Kilger on 7/6/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "SUTokenizer.h"
#include "SUSyntaxTree.h"
#include "SUType.h"

int main(int argc, const char * argv[]) {
	@autoreleasepool {
		for (int i = 0; i < 20000; i++) {
			SUList * tokens = SUTokenizeFile(argv[1]);
			SUSyntaxTree * tree = SUSyntaxTreeCreate(tokens);
			if (tokens)
				SURelease(tokens);
			if (tree)
				SURelease(tree);
		}
	}
    return 0;
}
