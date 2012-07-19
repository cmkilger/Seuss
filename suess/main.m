//
//  main.m
//  suess
//
//  Created by Cory Kilger on 7/6/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "SUTokenizer.h"
#include "SUProgram.h"
#include "SUString.h"
#include "SUList.h"
#include "SUType.h"
#include <sys/time.h>

#define ITER 10000

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        struct timeval startTime, endTime;
        gettimeofday(&startTime, NULL);
        
        for (int i = 0; i < ITER; i++) {
            SUString * file = SUStringCreate(argv[1]);
            SUList * tokens = SUTokenizeFile(file);
            SUList * errors = SUListCreate();
            SUProgram * program = SUProgramCreate(tokens, errors);
            if (tokens)
                SURelease(tokens);
            if (program)
                SURelease(program);
            if (file)
                SURelease(file);
            if (errors)
                SURelease(errors);
        }
        
        gettimeofday(&endTime, NULL);
        
        double time = (((endTime.tv_sec - startTime.tv_sec) * 1000000.0) + (endTime.tv_usec - startTime.tv_usec));
        printf("Average Time = %.0f microseconds\n", time/ITER);
    }
    return 0;
}
