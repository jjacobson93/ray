//
//  Lexer.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include "Lexer.h"
#include <math.h>
#include <iostream>

/*
 * Next char in the array.
 */
#define next (this->stash = this->source[this->offset++])

/*
 * Undo the previous char.
 */

#define undo (this->source[--this->offset] = this->stash)

/*
 * Assign token `t`.
 */

#define token(t) (this->tok->type = RAY_TOKEN_##t)

/*
 * Accept char `c` or undo and return 0.
 */

#define accept(c) (c == next ? c : (undo, 0))


/*
 * Convert hex digit `c` to a base 10 int,
 * returning -1 on failure.
 */

int hex(const char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

RayLexer::RayLexer(std::string source, std::string filename) {
    this->source = source;
    this->filename = filename;
    this->offset = 0;
    this->lineno = 0;
    this->tok = new RayToken::RayToken(RAY_TOKEN_ILLEGAL);
}

/*
 * Set error `msg` and assign ILLEGAL token.
 */
RayTokenType RayLexer::error(std::string err) {
    this->err = new std::string(err);
    return token(ILLEGAL);
}

/*
 * Scan identifier.
 */

int RayLexer::scanIdentifier(char c) {
    std::string s = "";
    token(ID);
    
    do {
        s += c;
    } while (isalpha(c = next) || isdigit(c) || '_' == c);
    undo;
    
    switch (s.length()) {
        case 2:
            if (0 == s.compare("if")) return token(IF);
            if (0 == s.compare("fn")) return token(FN);
            if (0 == s.compare("or")) return token(OP_OR);
            break;
        case 3:
            if (0 == s.compare("for")) return token(FOR);
            if (0 == s.compare("var")) return token(VAR);
            if (0 == s.compare("and")) return token(OP_AND);
            if (0 == s.compare("not")) return token(OP_NOT);
            break;
        case 4:
            if (0 == s.compare("true")) {
                token(BOOL);
                this->tok->value = s;
                return 1;
            }
            if (0 == s.compare("else")) return token(ELSE);
            if (0 == s.compare("loop")) return token(LOOP);
            break;
        case 5:
            if (0 == s.compare("false")) {
                token(BOOL);
                this->tok->value = s;
                return 1;
            }
            if (0 == s.compare("while")) return token(WHILE);
            if (0 == s.compare("until")) return token(UNTIL);
            break;
        default:
            if (0 == s.compare("return")) return token(RETURN);
            if (0 == s.compare("unless")) return token(UNLESS);
            if (0 == s.compare("extern")) return token(EXTERN);
            break;
    }
    
    this->tok->value = s;
    return 1;
}

/*
 * Scan string hex literal, returning -1 on invalid digits.
 */
int RayLexer::hexLiteral() {
    int a = hex(next);
    int b = hex(next);
    if (a > -1 && b > -1) return a << 4 | b;
    error("string hex literal \\x constains invalid digits");
    return -1;
}

/*
 * Scan string.
 */
int RayLexer::scanString(char quote) {
    char c;
    std::string s = "";
    token(STRING);
    
    while (quote != (c = next)) {
        switch (c) {
            case '\n': ++this->lineno; break;
            case '\\':
                switch (c = next) {
                    case 'a': c = '\a'; break;
                    case 'b': c = '\b'; break;
                    case 'e': c = '\e'; break;
                    case 'f': c = '\f'; break;
                    case 'n': c = '\n'; break;
                    case 'r': c = '\r'; break;
                    case 't': c = '\t'; break;
                    case 'v': c = '\v'; break;
                    case '"': c = '\"'; break;
                    case 'x':
                        if (-1 == (c = this->hexLiteral()))
                            return 0;
                }
                break;
                
            default:
                break;
        }
        s += c;
    }
    
    this->tok->value = s;
    return 1;
}

/*
 * Scan exponent
 */
int RayLexer::scanExpo(long n, long e, bool ftype) {
    char c;
    bool expo_type = false;
    long expo = 0;
    while (isdigit(c = next) || '+' == c || '-' == c) {
        if ('-' == c) {
            expo_type = 0;
            continue;
        }
        expo = expo * 10 + c - '0';
    }
    
    undo;
    if (!expo_type) expo *= -1;
    if (!ftype)
        this->tok->value = std::to_string(n * pow(10, expo));
    else
        this->tok->value = std::to_string(((double) n / e) * pow(10, expo));
    
    return 1;
}

/*
 * Scan floating-point number.
 */
int RayLexer::scanFloat(long n) {
    long e = 1;
    bool type = true;
    token(FLOAT);
    char c;
    while (isdigit(c = next) || '_' == c || 'e' == c || 'E' == c) {
        if ('_' == c) continue;
        else if ('e' == c || 'E' == c) return this->scanExpo(n, e, type);
        n = n * 10 + c - '0';
        e *= 10;
    }
    undo;
    this->tok->value = std::to_string((double)n / e);
    return 1;
}

/*
 * Scan integer.
 */
int RayLexer::scanInt(long n, char c) {
    do {
        if ('_' == c) continue;
        else if ('.' == c) return this->scanFloat(n);
        else if ('e' == c || 'E' == c) return this->scanExpo(n, 1, false);
        n = n * 10 + c - '0';
    } while (isdigit(c = next) || '_' == c || '.' == c || 'e' == c || 'E' == c);
    undo;
    this->tok->value = std::to_string(n);
    return 1;
}

/*
 * Scan hex number.
 */
int RayLexer::scanHex(long n){
    char c;
    switch (c = next) {
        case 'x':
            if (!isdigit(c = next)) {
                error("hex literal expects one or more digits");
                return 0;
            } else {
                do n = n << 4 | hex(c);
                while (isxdigit(c = next));
            }
            this->tok->value = std::to_string(n);
            undo;
            return 1;
        default:
            undo;
            c = '0';
            return this->scanInt(n, c);
    }
}

/*
 * Scan number.
 */
int RayLexer::scanNumber(char c) {
    token(INT);
    if (c == '0') return this->scanHex(0);
    return this->scanInt(0, c);
}

/*
 * Scan the next token in the stream, returns 0
 * on EOS, ILLEGAL token, or a syntax error.
 */
int RayLexer::scan() {
    char c;
    this->tok->type = RAY_TOKEN_ILLEGAL;
    
    while (true) {
        switch (c = next) {
            case ' ':
            case '\t': continue;
            case '(': return token(LPAREN);
            case ')': return token(RPAREN);
            case '{': return token(LBRACE);
            case '}': return token(RBRACE);
            case '[': return token(LBRACK);
            case ']': return token(RBRACK);
            case ',': return token(COMMA);
            case ';': return token(SEMICOLON);
            case '.': return token(OP_DOT);
            case '%': return token(OP_MOD);
            case '^': return token(OP_BIT_XOR);
            case '~': return token(OP_BIT_NOT);
            case '?':
                switch (c = next) {
                    case '.': return token(OP_QDOT);
                    default: return undo, token(QMARK);
                }
            case ':': return token(COLON);
            case '+':
                switch (next) {
                    case '+': return token(OP_INCR);
                    case '=': return token(OP_PLUS_ASSIGN);
                    default: return undo, token(OP_PLUS);
                }
            case '-':
                switch (next) {
                    case '-': return token(OP_DECR);
                    case '=': return token(OP_MINUS_ASSIGN);
                    case '>': return token(ARROW);
                    default: return undo, token(OP_MINUS);
                }
            case '*':
                switch (next) {
                    case '=': return token(OP_MUL_ASSIGN);
                    case '*': return token(OP_POW);
                    default: return undo, token(OP_MUL);
                }
            case '/':
                return '=' == next
                ? token(OP_DIV_ASSIGN)
                : (undo, token(OP_DIV));
            case '!':
                return '=' == next
                ? token(OP_NEQ)
                : (undo, token(OP_NOT));
            case '=':
                return '=' == next
                ? token(OP_EQ)
                : (undo, token(OP_ASSIGN));
            case '&':
                switch (next) {
                    case '&':
                        return '=' == next
                        ? token(OP_AND_ASSIGN)
                        : (undo, token(OP_AND));
                    default:
                        return undo, token(OP_BIT_AND);
                }
            case '|':
                switch (next) {
                    case '|':
                        return '=' == next
                        ? token(OP_OR_ASSIGN)
                        : (undo, token(OP_OR));
                    default:
                        return undo, token(OP_BIT_OR);
                }
            case '<':
                switch (next) {
                    case '=': return token(OP_LTE);
                    case '<': return token(OP_BIT_SHL);
                    default: return undo, token(OP_LT);
                }
            case '>':
                switch (next) {
                    case '=': return token(OP_GTE);
                    case '>': return token(OP_BIT_SHR);
                    default: return undo, token(OP_GT);
                }
            case '#':
                while ((c = next) != '\n' && c) ; undo;
                continue;
            case '\n':
            case '\r':
                ++this->lineno;
                continue;
            case '"':
            case '\'':
                return this->scanString(c);
            case 0:
                token(EOS);
                return 0;
            default:
                if (isalpha(c) || '_' == c) return this->scanIdentifier(c);
                if (isdigit(c) || '.' == c) return this->scanNumber(c);
                error("illegal character");
                return 0;
        }
    }
}
