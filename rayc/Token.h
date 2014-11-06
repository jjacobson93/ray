//
//  Token.h
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__Token__
#define __rayc__Token__

#include <string>

/**
 * Token values
 */
typedef enum {
    RAY_TOKEN_ILLEGAL,
    RAY_TOKEN_EOS,
    RAY_TOKEN_ID,
    RAY_TOKEN_INT,
    RAY_TOKEN_FLOAT,
    RAY_TOKEN_STRING,
    RAY_TOKEN_CHAR,
    RAY_TOKEN_BOOL,
    RAY_TOKEN_FN,
    RAY_TOKEN_WHILE,
    RAY_TOKEN_UNTIL,
    RAY_TOKEN_LOOP,
    RAY_TOKEN_IF,
    RAY_TOKEN_UNLESS,
    RAY_TOKEN_ELSE,
    RAY_TOKEN_FOR,
    RAY_TOKEN_IN,
    RAY_TOKEN_VAR,
    RAY_TOKEN_RETURN,
    RAY_TOKEN_LBRACE,
    RAY_TOKEN_RBRACE,
    RAY_TOKEN_LPAREN,
    RAY_TOKEN_RPAREN,
    RAY_TOKEN_LBRACK,
    RAY_TOKEN_RBRACK,
    RAY_TOKEN_COLON,
    RAY_TOKEN_ARROW,
    RAY_TOKEN_QMARK,
    RAY_TOKEN_SEMICOLON,
    RAY_TOKEN_COMMA,
    RAY_TOKEN_OP_DOT,
    RAY_TOKEN_OP_QDOT,
    RAY_TOKEN_OP_NOT,
    RAY_TOKEN_OP_REF,
    RAY_TOKEN_OP_PLUS,
    RAY_TOKEN_OP_INCR,
    RAY_TOKEN_OP_MINUS,
    RAY_TOKEN_OP_DECR,
    RAY_TOKEN_OP_MUL,
    RAY_TOKEN_OP_DIV,
    RAY_TOKEN_OP_MOD,
    RAY_TOKEN_OP_POW,
    RAY_TOKEN_OP_GT,
    RAY_TOKEN_OP_LT,
    RAY_TOKEN_OP_GTE,
    RAY_TOKEN_OP_LTE,
    RAY_TOKEN_OP_EQ,
    RAY_TOKEN_OP_NEQ,
    RAY_TOKEN_OP_AND,
    RAY_TOKEN_OP_OR,
    RAY_TOKEN_OP_ASSIGN,
    RAY_TOKEN_OP_PLUS_ASSIGN,
    RAY_TOKEN_OP_MINUS_ASSIGN,
    RAY_TOKEN_OP_MUL_ASSIGN,
    RAY_TOKEN_OP_DIV_ASSIGN,
    RAY_TOKEN_OP_AND_ASSIGN,
    RAY_TOKEN_OP_OR_ASSIGN,
    RAY_TOKEN_OP_BIT_AND,
    RAY_TOKEN_OP_BIT_OR,
    RAY_TOKEN_OP_BIT_XOR,
    RAY_TOKEN_OP_BIT_NOT,
    RAY_TOKEN_OP_BIT_SHL,
    RAY_TOKEN_OP_BIT_SHR
} RayTokenType;

static std::string tokenStrings[] = {
    "illegal",
    "end-of-source",
    "id",
    "int",
    "float",
    "string",
    "char",
    "bool",
    "fn",
    "while",
    "until",
    "loop",
    "if",
    "unless",
    "else",
    "for",
    "in",
    "var",
    "return",
    "{",
    "}",
    "(",
    ")",
    "[",
    "]",
    ":",
    "->",
    "?",
    ";",
    ",",
    ".",
    "?.",
    "!",
    "&",
    "+",
    "++",
    "-",
    "--",
    "*",
    "/",
    "%",
    "**",
    ">",
    "<",
    ">=",
    "<=",
    "==",
    "!=",
    "&&",
    "||",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "&&=",
    "||=",
    "&",
    "|",
    "^",
    "~",
    "<<",
    ">>"
};

static std::string keywords[] = {
    "int",
    "float",
    "string",
    "char",
    "bool",
    "fn",
    "while",
    "until",
    "loop",
    "if",
    "unless",
    "else",
    "for",
    "in",
    "var",
    "return"
};

/**
 * Token class
 */
class RayToken {
    int len;

public:
    /* values */
    RayTokenType type;
    std::string value;
    
    /* member functions */
    RayToken();
    RayToken(RayTokenType type);
    std::string inspect();
    char asChar();
    std::string asString();
    double asFloat();
    long asInt();
    bool asBool();
};

#endif /* defined(__rayc__Token__) */
