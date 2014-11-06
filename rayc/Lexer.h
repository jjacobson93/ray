//
//  Lexer.h
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__Lexer__
#define __rayc__Lexer__

#include <string>
#include "Token.h"

class RayLexer {

public:
    std::string *err;
    char stash;
    long lineno;
    long offset;
    std::string source;
    std::string filename;
    RayToken *tok;
    
    RayLexer(std::string source, std::string filename);
    RayTokenType error(std::string err);
    int hexLiteral();
    int scan();
    int scanIdentifier(char c);
    int scanString(char quote);
    int scanNumber(char c);
    int scanExpo(long n, long e, bool ftype);
    int scanFloat(long n);
    int scanInt(long n, char c);
    int scanHex(long n);
};

#endif /* defined(__rayc__Lexer__) */
