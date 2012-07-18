//
//  SUTokenizer.h
//  suess
//
//  Created by Cory Kilger on 7/8/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef suess_SUTokenizer_h
#define suess_SUTokenizer_h

typedef enum {
    SUTokenTypeWord,
    SUTokenTypeString,
    SUTokenTypeNumber,
    SUTokenTypePlus,
    SUTokenTypeMinus,
    SUTokenTypeMultiply,
    SUTokenTypeDivide,
    SUTokenTypeExponent,
    SUTokenTypeOpenParenthesis,
    SUTokenTypeCloseParenthesis,
    SUTokenTypeColon,
    SUTokenTypePeriod,
//    SUTokenTypeTrue,
//    SUTokenTypeFalse,
    SUTokenTypeStartFunctionDefinition,
    SUTokenTypeEndFunctionDefinition,
    SUTokenTypeUndefined,
} SUTokenType;

struct suess_list;
struct suess_string;

typedef struct suess_token SUToken;

SUToken * SUTokenCreate(SUTokenType tokenType, const char * value, struct suess_string * file, unsigned int line);
SUTokenType SUTokenGetType(SUToken * token);
struct suess_string *  SUTokenGetValue(SUToken * token);
struct suess_list * SUTokenizeFile(struct suess_string * filename);

#endif
