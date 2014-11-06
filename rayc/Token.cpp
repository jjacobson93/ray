//
//  Token.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <iostream>
#include "Token.h"

RayToken::RayToken() {}

RayToken::RayToken(RayTokenType type) {
    this->type = type;
}

std::string RayToken::inspect() {
    std::string s;
    s += tokenStrings[this->type];
    s += ' ';
    switch (this->type) {
        case RAY_TOKEN_INT:
        case RAY_TOKEN_FLOAT:
        case RAY_TOKEN_BOOL:
        case RAY_TOKEN_STRING:
        case RAY_TOKEN_CHAR:
        case RAY_TOKEN_ID:
            s += this->asString();
        default:
            break;
    }
    return s;
}

bool RayToken::asBool() {
    if (this->value.compare("true")) {
        return true;
    } else {
        return false;
    }
}

char RayToken::asChar() {
    return this->value[0];
}

double RayToken::asFloat() {
    return std::stod(this->value);
}

long RayToken::asInt() {
    return std::stol(this->value);
}

std::string RayToken::asString() {
    return this->value;
}
