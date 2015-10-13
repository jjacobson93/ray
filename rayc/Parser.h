//
//  Parser.h
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__Parser__
#define __rayc__Parser__

#include "AST.h"
#include "Token.h"
#include "Lexer.h"
#include <vector>
#include <string>

class RayParser {
    RayLexer *lex;
    RayToken *la;
    RayToken *tmp;
    RayToken lb;
    std::string ctx;
    std::string *err;
    int in_args;
    
public:
    RayParser(RayLexer *lex);
    
    std::nullptr_t error(std::string err);
    void reportError();
    Node *paren_expr();
    bool arg_list(ArrayNode *arr, RayToken delim);
    Node *array_expr();
    bool hash_pairs(HashNode *hash, RayToken delim);
    Node *hash_expr();
    Node *primary_expr();
    Node *pow_expr();
    Node *postfix_expr();
    Node *unary_expr();
    Node *multiplicative_expr();
    Node *additive_expr();
    Node *shift_expr();
    Node *relational_expr();
    Node *equality_expr();
    Node *bitwise_and_expr();
    Node *bitwise_xor_expr();
    Node *bitwise_or_expr();
    Node *logical_and_expr();
    Node *logical_or_expr();
    Node *type_expr();
    std::vector<Node*> *function_params();
    //Node *function_expr();
    Node *slot_access_expr();
    ArgsNode *call_args();
    Node *call_expr();
    Node *assignment_expr();
    Node *not_expr();
    Node *expr();
    Node *expr_stmt();
    Node *function_stmt();
    Node *extern_stmt();
    Node *if_stmt();
    Node *while_stmt();
    Node *return_stmt();
    Node *stmt();
    Node *simple_stmt();
    Node *compound_stmt();
    BlockNode *block();
    BlockNode *program();
    BlockNode *parse();
};

#endif /* defined(__rayc__Parser__) */
