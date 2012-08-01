//
//  SUTokenizer.h
//  seuss
//
//  Created by Cory Kilger on 7/8/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#ifndef seuss_SUTokenizer_h
#define seuss_SUTokenizer_h

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

struct seuss_list;
struct seuss_string;

typedef struct seuss_token SUToken;

SUToken * SUTokenCreate(SUTokenType tokenType, const char * value, struct seuss_string * file, unsigned int line);
SUTokenType SUTokenGetType(SUToken * token);
struct seuss_string * SUTokenGetValue(SUToken * token);
struct seuss_string * SUTokenGetFile(SUToken * token);
unsigned int SUTokenGetLine(SUToken * token);

struct seuss_list * SUTokenizeFile(struct seuss_string * filename);
struct seuss_list * SUTokenizeData(const char * data, long filesize, struct seuss_string * filename);

#endif
