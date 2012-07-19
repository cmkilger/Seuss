//
//  main.m
//  suess
//
//  Created by Cory Kilger on 7/6/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "SUTokenizer.h"
#include "SUIterator.h"
#include "SUProgram.h"
#include "SUString.h"
#include "SUError.h"
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
            
            int shouldRun = 1;
            SUError * error = NULL;
            SUIterator * errorIterator = SUListCreateIterator(errors);
            while ((error = SUIteratorNext(errorIterator))) {
                const char * type = "warning";
                if (SUErrorGetType(error) == SUErrorTypeError) {
                    shouldRun = 0;
                    type = "error";
                }
                const char * file = SUStringGetCString(SUErrorGetFile(error));
                unsigned int line = SUErrorGetLine(error);
                const char * message = SUStringGetCString(SUErrorGetMessage(error));
                printf("%s: %s:%d: %s\n", type, file, line, message);
            }
            
            if (shouldRun) {
                // TODO: Execute program
            }
            
            SURelease(errorIterator);
            SURelease(tokens);
            SURelease(program);
            SURelease(file);
            SURelease(errors);
        }
        
        gettimeofday(&endTime, NULL);
        
        double time = (((endTime.tv_sec - startTime.tv_sec) * 1000000.0) + (endTime.tv_usec - startTime.tv_usec));
        printf("Average Time = %.0f microseconds\n", time/ITER);
    }
    return 0;
}
