//
//  SUTokenizer.c
//  seuss
//
//  Created by Cory Kilger on 7/8/12.
//  Copyright (c) 2012 Cory Kilger. All rights reserved.
//

#include "SUTokenizer.h"
#include "SUString.h"
#include "SUList.h"
#include "SUTypeInternal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct seuss_token {
    SUType __base;
    SUTokenType type;
    SUString * value;
    SUString * file;
    unsigned int line;
};

void seuss_token_free(SUTypeRef type) {
    SUToken * token = type;
    SURelease(token->value);
    SURelease(token->file);
    seuss_free(type);
}

SUToken * SUTokenCreate(SUTokenType tokenType, const char * value, SUString * file, unsigned int line) {
    SUToken * token = malloc(sizeof(SUToken));
    SUInitialize(token, NULL, NULL, seuss_token_free);
    token->type = tokenType;
    token->value = SUStringCreate(value);
    token->file = SURetain(file);
    token->line = line;
    return token;
}

SUTokenType SUTokenGetType(SUToken * token) {
    return token->type;
}

SUString *  SUTokenGetValue(SUToken * token) {
    return token->value;
}

struct seuss_string * SUTokenGetFile(SUToken * token) {
    return token->file;
}

unsigned int SUTokenGetLine(SUToken * token) {
    return token->line;
}

SUList * SUTokenizeFile(SUString * filename) {
    FILE * file = fopen(SUStringGetCString(filename), "r");
    
    if (!file)
        return NULL;
    
    // File size
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);
    
    // Allocate memory for file
    char * buffer = malloc(sizeof(char) * filesize);
    if (buffer == NULL) {
        // TODO: error
        return NULL;
    }
    
    // Read file
    size_t result = fread(buffer, 1, filesize, file);
    if (result != filesize) {
        // TODO: error
        return NULL;
    }
    
    // Tokenize
    SUList * tokens = SUTokenizeData(buffer, filesize, filename);
    
    // Cleanup
    fclose(file);
    free(buffer);
    
    return tokens;
}

SUList * SUTokenizeData(const char * buffer, long filesize, SUString * filename) {
    // Parse
    unsigned int line = 1;
    SUList * tokens = SUListCreate();
    for (size_t i = 0; i < filesize; i++) {
        SUToken * token = NULL;
        char character = buffer[i];
        switch (character) {
            case '\n': {
                line++;
            } break;
                
            case '+': {
                token = SUTokenCreate(SUTokenTypePlus, "+", filename, line);
            } break;
                
            case '-': {
                token = SUTokenCreate(SUTokenTypeMinus, "-", filename, line);
            } break;
                
            case '*': {
                token = SUTokenCreate(SUTokenTypeMultiply, "*", filename, line);
            } break;
                
            case '/': {
                token = SUTokenCreate(SUTokenTypeDivide, "/", filename, line);
            } break;
                
            case '^': {
                token = SUTokenCreate(SUTokenTypeExponent, "^", filename, line);
            } break;
                
            case '(': {
                token = SUTokenCreate(SUTokenTypeOpenParenthesis, "(", filename, line);
            } break;
                
            case ')': {
                token = SUTokenCreate(SUTokenTypeCloseParenthesis, ")", filename, line);
            } break;
                
            case ':': {
                token = SUTokenCreate(SUTokenTypeColon, ":", filename, line);
            } break;
                
            case '.': {
                token = SUTokenCreate(SUTokenTypePeriod, ".", filename, line);
            } break;    
                
            default: {
                // space
                if (isspace(character)) {
                    break;
                }
                
                /* Disabling true and false for now. These will likely become predefined variables instead.
                 
                 // true
                 if ((character == 't' || character == 'T') && i+3 < filesize) {
                 char value[4];
                 for (long j = 0; j < 4; j++)
                 value[j] = tolower(buffer[i+j]);
                 if (strncmp("true", value, 4) == 0) {
                 token = SUTokenCreate(SUTokenTypeTrue, "true", filename, line);
                 i += 3;
                 break;
                 }
                 }
                 
                 
                 // false
                 if ((character == 'f' || character == 'F') && i+4 < filesize) {
                 char value[5];
                 for (long j = 0; j < 5; j++)
                 value[j] = tolower(buffer[i+j]);
                 if (strncmp("false", value, 5) == 0) {
                 token = SUTokenCreate(SUTokenTypeFalse, "false", filename, line);
                 i += 4;
                 break;
                 }
                 }
                 
                 */
                
                // Start function
                if ((character == 'd' || character == 'D') && i+14 < filesize) {
                    char value[15];
                    for (long j = 0; j < 15; j++)
                        value[j] = tolower(buffer[i+j]);
                    if (strncmp("define function", value, 15) == 0) {
                        token = SUTokenCreate(SUTokenTypeStartFunctionDefinition, "define function", filename, line);
                        i += 14;
                        break;
                    }
                }
                
                // End function
                if ((character == 'e' || character == 'E') && i+12 < filesize) {
                    char value[13];
                    for (long j = 0; j < 13; j++)
                        value[j] = tolower(buffer[i+j]);
                    if (strncmp("end function.", value, 13) == 0) {
                        token = SUTokenCreate(SUTokenTypeEndFunctionDefinition, "end function.", filename, line);
                        i += 12;
                        break;
                    }
                }
                
                // Word
                if (isalpha(character)) {
                    long j = 1;
                    while (i+j < filesize && isalpha(buffer[i+j]))
                        j++;
                    char * value = malloc(sizeof(char)*(j+1));
                    strncpy(value, &(buffer[i]), j);
                    value[j] = 0;
                    token = SUTokenCreate(SUTokenTypeWord, value, filename, line);
                    free(value);
                    i += j-1;
                    break;
                }
                
                // Number
                if (isdigit(character)) {
                    long j = 1;
                    int decimal = 0;
                    while (i+j < filesize && (isdigit(buffer[i+j]) || (!decimal && buffer[i+j] == '.'))) {
                        if (buffer[i+j] == '.')
                            decimal = 1;
                        j++;
                    }
                    char * value = calloc(sizeof(char), j+1);
                    strncpy(value, &(buffer[i]), j);
                    token = SUTokenCreate(SUTokenTypeNumber, value, filename, line);
                    i += j-1;
                    break;
                }
                
                // String
                if (character == '"') {
                    long j = 1;
                    int escaping = 0;
                    int lines = 0;
                    while (i+j < filesize && (escaping || buffer[i+j] != '"')) {
                        if (escaping)
                            escaping = 0;
                        else if (buffer[i+j] == '\\')
                            escaping = 1;
                        if (buffer[i+j] == '\n')
                            lines++;
                        j++;
                    }
                    j++;
                    char * value = calloc(sizeof(char), j-1);
                    strncpy(value, &(buffer[i+1]), j-2);
                    token = SUTokenCreate(SUTokenTypeString, value, filename, line);
                    free(value);
                    i += j-1;
                    line += lines;
                    break;
                }
                
            } break;
        }
        
        if (token) {
            SUListAddValue(tokens, token);
            SURelease(token);
        }
    }
    
    return tokens;
}
