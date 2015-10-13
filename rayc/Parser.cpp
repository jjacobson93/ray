//
//  Parser.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include "Parser.h"
#include <iostream>

#define DEBUG_PARSER 1

#ifdef DEBUG_PARSER
#define debug(name) \
std::cerr << name << std::endl; \
std::cerr << this->lex->tok->inspect() << std::endl;
#else
#define debug(name)
#endif

/*
 * Accept a token, advancing the lexer.
 */
#define accept(t) \
(peek->type == RAY_TOKEN_##t \
? (this->lb = *this->la, this->la = nullptr, &this->lb) \
: 0)

/*
 * Return the next token.
 */

#define advance (this->lex->scan(), this->lex->tok)

/*
 * Previous token look-behind.
 */

#define prev (&this->lb)

/*
 * Return the next token, previously peeked token.
 */

#define next \
(this->la \
? (this->tmp = this->la, this->la = nullptr, this->tmp) \
: advance)

/*
 * Single token look-ahead.
 */

#define peek (this->la ? this->la : (this->la = advance))

/*
 * Check if the next token is `t`.
 */

#define is(t) (peek->type == RAY_TOKEN_##t)

/*
 * Set error context `str`, used in error reporting.
 */

#define context(str) (this->ctx = str)

/*
 * Set node location
 */
#define location(n) n->lineno = this->lex->lineno; n->offset = this->lex->offset;

RayParser::RayParser(RayLexer *lex) : lb(RAY_TOKEN_ILLEGAL) {
    this->lex = lex;
    this->in_args = 0;
    this->err = nullptr;
}

std::nullptr_t RayParser::error(std::string err) {
    this->err = (this->err ? this->err : new std::string(err));
    return nullptr;
}

void RayParser::reportError() {
    std::string err, type = "parse";
    RayLexer *lex = this->lex;
    
    // error message
    if (this->err) {
        err = *this->err;
    } else if (lex->err) {
        err = *lex->err;
        type = "syntax";
    } else {
        err = "unexpected token '" + tokenStrings[lex->tok->type] + "'";
    }
    
    std::cerr << "ray(" << lex->filename << ":" << lex->lineno << "). " << type << " error in " << this->ctx << ", " << err << "." << std::endl;
}

/*
 * '(' expr ')'
 */
Node *RayParser::paren_expr() {
    Node *node;
    debug("paren_expr");
    
    if (!accept(LPAREN)) return nullptr;
    if (!(node = this->expr())) return nullptr;
    if (!accept(RPAREN)) return this->error("expression missing closing ')'");
    return node;
}

/*
 *   expr ','?
 * | expr ',' arg_list
 */
bool RayParser::arg_list(ArrayNode *arr, RayToken delim) {
    // trailing ','
    if (delim.type == peek->type) return true;
    
    // expr
    Node *val;
    if (!(val = this->expr())) return false;
    
    arr->vals->push_back(val);
    
    // ',' arg_list
    if (accept(COMMA)) {
        if (!this->arg_list(arr, delim)) return false;
    }
    
    return true;
}

/*
 * '[' arg_list? ']'
 */
Node *RayParser::array_expr() {
    ArrayNode *node = new ArrayNode();
    debug("array_expr");
    
    location(node);
    
    if (!accept(LBRACK)) return nullptr;
    context("array");
    if (!this->arg_list(node, RAY_TOKEN_RBRACK)) return nullptr;
    if (!accept(RBRACK)) return error("array missing closing ']'");
    return (Node *)node;
}

/*
 *   id ':' expr
 * | id ':' expr ',' hash_pairs
 */
bool RayParser::hash_pairs(HashNode *hash, RayToken delim) {
    // trailing ','
    if (delim.type == peek->type) return 1;
    
    // id
    if (!is(ID)) return error("hash pair key expected"), false;
    std::string id = next->asString();
    
    // :
    if (!accept(COLON)) return error("hash pair ':' missing"), false;
    
    // expr
    Node *val;
    if (!(val = this->expr())) return false;
    (*hash->vals)[id] = val;
    
    // ',' hash_pairs
    if (accept(COMMA)) {
        if (!this->hash_pairs(hash, delim)) return false;
    }
    
    
    return true;
}

/*
 * '{' hash_pairs? '}'
 */
Node *RayParser::hash_expr() {
    HashNode *node = new HashNode();
    
    debug("hash_expr");
    
    location(node);
    
    if (!accept(LBRACE)) return nullptr;
    context("hash");
    if (!this->hash_pairs(node, RAY_TOKEN_RBRACE)) return nullptr;
    if (!accept(RBRACE)) return error("hash missing closing '}'");
    return (Node*)node;
}

/*
 *   id
 * | int
 * | float
 * | string
 * | bool
 * | char
 * | array
 * | hash
 * | function_stmt
 * | extern_stmt
 * | paren_expr
 */
Node *RayParser::primary_expr() {
    debug("primary_expr");
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    switch (peek->type) {
        case RAY_TOKEN_ID: {
            Node *n = (Node *) new IDNode(next->asString());
            n->lineno = lineno; n->offset = offset;
            return n;
        }
        case RAY_TOKEN_INT: {
            Node *n = (Node *) new IntNode(next->asInt());
            n->lineno = lineno; n->offset = offset;
            return n;
        }
        case RAY_TOKEN_FLOAT: {
            Node *n = (Node *) new FloatNode(next->asFloat());
            n->lineno = lineno; n->offset = offset;
            return n;
        }
        case RAY_TOKEN_STRING: {
            Node *n = (Node *) new StringNode(new std::string(next->asString()));
            n->lineno = lineno; n->offset = offset;
            return n;
        }
        case RAY_TOKEN_CHAR: {
            Node *n = (Node *) new CharNode(next->asChar());
            n->lineno = lineno; n->offset = offset;
            return n;
        }
        case RAY_TOKEN_BOOL: {
            Node *n = (Node *) new BoolNode(next->asBool());
            n->lineno = lineno; n->offset = offset;
            return n;
        }
        case RAY_TOKEN_LBRACK:
            return this->array_expr();
        case RAY_TOKEN_LBRACE:
            return this->hash_expr();
        case RAY_TOKEN_FN:
            return this->function_stmt();
        case RAY_TOKEN_EXTERN:
            return this->extern_stmt();
        default:
            return this->paren_expr();
    }
}

/*
 *   call_expr
 * | call_expr '**' call_expr
 */
Node *RayParser::pow_expr() {
    Node *node, *right;
    debug("pow_expr");
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    if (!(node = this->call_expr())) return nullptr;
    if (accept(OP_POW)) {
        context("** operation");
        if ((right = this->call_expr())) {
            Node *n = (Node *) new BinaryOpNode(RAY_TOKEN_OP_POW, node, right);
            n->lineno = lineno;
            n->offset = offset;
            return n;
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 *   pow_expr
 * | pow_expr '++'
 * | pow_expr '--'
 */

Node *RayParser::postfix_expr() {
    Node *node;
    debug("postfix_expr");
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    if (!(node = this->pow_expr())) return nullptr;
    if (accept(OP_INCR) || accept(OP_DECR)) {
        Node *n = (Node *) new UnaryOpNode(prev->type, node, true);
        n->lineno = lineno;
        n->offset = offset;
        return n;
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 *   '++' unary_expr
 * | '--' unary_expr
 * | '~' unary_expr
 * | '+' unary_expr
 * | '-' unary_expr
 * | '!' unary_expr
 * | primary_expr
 */

Node *RayParser::unary_expr() {
    debug("unary_expr");
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    if (accept(OP_INCR)
        || accept(OP_DECR)
        || accept(OP_BIT_NOT)
        || accept(OP_PLUS)
        || accept(OP_MINUS)
        || accept(OP_NOT)) {
        Node *n = (Node *) new UnaryOpNode(prev->type, this->unary_expr(), false);
        n->lineno = lineno;
        n->offset = offset;
        return n;
    }
    return this->postfix_expr();
}

/*
 * unary_expr (('* | '/' | '%') unary_expr)*
 */

Node *RayParser::multiplicative_expr() {
    RayTokenType op;
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("multiplicative_expr");
    if (!(node = this->unary_expr())) return nullptr;
    while (accept(OP_MUL) || accept(OP_DIV) || accept(OP_MOD)) {
        op = prev->type;
        context("multiplicative operation");
        if ((right = this->unary_expr())) {
            node = (Node *) new BinaryOpNode(op, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * multiplicative_expr (('+ | '-') multiplicative_expr)*
 */

Node *RayParser::additive_expr() {
    RayTokenType op;
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("additive_expr");
    if (!(node = this->multiplicative_expr())) return nullptr;
    while (accept(OP_PLUS) || accept(OP_MINUS)) {
        op = prev->type;
        context("additive operation");
        if ((right = this->multiplicative_expr())) {
            node = (Node *) new BinaryOpNode(op, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * additive_expr (('<<' | '>>') additive_expr)*
 */

Node *RayParser::shift_expr() {
    RayTokenType op;
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("shift_expr");
    if (!(node = this->additive_expr())) return nullptr;
    while (accept(OP_BIT_SHL) || accept(OP_BIT_SHR)) {
        op = prev->type;
        context("shift operation");
        if ((right = this->additive_expr())) {
            node = (Node *) new BinaryOpNode(op, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * shift_expr (('<' | '<=' | '>' | '>=') shift_expr)*
 */

Node *RayParser::relational_expr() {
    RayTokenType op;
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("relational_expr");
    if (!(node = this->shift_expr())) return nullptr;
    while (accept(OP_LT) || accept(OP_LTE) || accept(OP_GT) || accept(OP_GTE)) {
        op = prev->type;
        context("relational operation");
        if ((right = this->shift_expr())) {
            node = (Node *) new BinaryOpNode(op, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * relational_expr (('==' | '!=') relational_expr)*
 */

Node *RayParser::equality_expr() {
    RayTokenType op;
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("equality_expr");
    if (!(node = this->relational_expr())) return nullptr;
    while (accept(OP_EQ) || accept(OP_NEQ)) {
        op = prev->type;
        context("equality operation");
        if ((right = this->relational_expr())) {
            node = (Node *) new BinaryOpNode(op, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * equality_expr ('&' equality_expr)*
 */

Node *RayParser::bitwise_and_expr() {
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("bitwise_and_expr");
    if (!(node = this->equality_expr())) return nullptr;
    while (accept(OP_BIT_AND)) {
        context("& operation");
        if ((right = this->equality_expr())) {
            node = (Node *) new BinaryOpNode(RAY_TOKEN_OP_BIT_AND, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * bitwise_and_expr ('^' bitwise_and_expr)*
 */

Node *RayParser::bitwise_xor_expr() {
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("bitwise_xor_expr");
    if (!(node = this->bitwise_and_expr())) return nullptr;
    while (accept(OP_BIT_XOR)) {
        context("^ operation");
        if ((right = this->bitwise_and_expr())) {
            node = (Node *) new BinaryOpNode(RAY_TOKEN_OP_BIT_XOR, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * bitwise_xor_expr ('|' bitwise_xor_expr)*
 */

Node *RayParser::bitwise_or_expr() {
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("bitswise_or_expr");
    if (!(node = this->bitwise_xor_expr())) return nullptr;
    while (accept(OP_BIT_OR)) {
        context("| operation");
        if ((right = this->bitwise_xor_expr())) {
            node = (Node *) new BinaryOpNode(RAY_TOKEN_OP_BIT_OR, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * bitswise_or_expr ('&&' bitswise_or_expr)*
 */

Node *RayParser::logical_and_expr() {
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("logical_and_expr");
    if (!(node = this->bitwise_or_expr())) return nullptr;
    while (accept(OP_AND)) {
        context("&& operation");
        if ((right = this->bitwise_or_expr())) {
            node = (Node *) new BinaryOpNode(RAY_TOKEN_OP_AND, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * logical_and_expr ('||' logical_and_expr)*
 */

Node *RayParser::logical_or_expr() {
    Node *node, *right;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    debug("logical_or_expr");
    if (!(node = this->logical_and_expr())) return nullptr;
    
    // '||'
    while (accept(OP_OR)) {
        context("|| operation");
        if ((right = this->logical_and_expr())) {
            node = (Node *) new BinaryOpNode(RAY_TOKEN_OP_OR, node, right);
        } else {
            return error("missing right-hand expression");
        }
    }
    
    // '&'
    //    if (accept(OP_FORK)) {
    //        ray_id_node_t *id = ray_id_node_new("fork");
    //        CallNode *call = new CallNode((Node *) id);
    //        ray_vec_push(call->args->vec, ray_node(node));
    //        node = (Node *) call;
    //    }
    
    
    
    node->lineno = lineno;
    node->offset = offset;
    return node;
}

/*
 * id | '(' type_expr (',' type_expr)+ ')'
 */
Node *RayParser::type_expr() {
    std::vector<Node*> *types = nullptr;
    Node *node = nullptr;
    long lineno = this->lex->lineno;
    long offset = this->lex->offset;
    
    if (is(ID)) {
        types = new std::vector<Node*>();
        Node *idnode = (Node *)new IDNode(next->asString());
        idnode->lineno = lineno;
        idnode->offset = offset;
        types->push_back(idnode);
        
        Node *typeNode = (Node *)new TypeNode(types);
        typeNode->lineno = lineno;
        typeNode->offset = offset;
        return typeNode;
    } else if (accept(LPAREN)) { // '('
        types = new std::vector<Node*>();
        // type_expr
        if (!(node = this->type_expr())) return error("inavlid types");
        types->push_back(node);
        
        do {
            // ','
            if (!accept(COMMA)) return error("missing ',' after type");
            
            // type_expr
            if (!(node = this->type_expr())) return error("invalid type");
            types->push_back(node);
            
        } while (!accept(RPAREN));
        
        Node *typeNode = (Node *)new TypeNode(types);
        typeNode->lineno = lineno;
        typeNode->offset = offset;
        return typeNode;
    }
    
    return nullptr;
    
}

/*
 * (id ':' type ('=' expr)? (',' id ':' type ('=' expr)?)*)
 */

std::vector<Node*> *RayParser::function_params() {
    std::vector<Node*> *params = new std::vector<Node*>();
    Node *type = nullptr;
//    long lineno = this->lex->lineno;
//    long offset = this->lex->offset;
    
    debug("params");
    context("function params");
    
    if (!is(ID)) return params;
    
    do {
        // id
        if (!is(ID)) return this->error("missing identifier");
        std::string id = next->value;
        
        // ':' type_expr
        if (!accept(COLON)) return error("missing parameter type");
        if (!(type = this->type_expr())) return error("missing parameter type expression");
        
        // ('=' expr)?
        Node *param;
        if (accept(OP_ASSIGN)) {
            Node *val = this->expr();
            if (!val) return nullptr;
            param = (Node *) new DeclNode(id, type, val);
        } else {
            param = (Node *) new DeclNode(id, type, nullptr);
        }
        
        params->push_back(param);
    } while (accept(COMMA));
    
    return params;
}

/*
 * ':' params? block
 */

//Node *RayParser::function_expr() {
//    BlockNode *body;
//    std::vector<Node*> *params;
//    debug("function_expr");
//
//    // ':'
//    if (accept(COLON)) {
//        // params?
//        if (!(params = this->function_params())) return nullptr;
//        context("function");
//
//        // block
//        if ((body = this->block())) {
//            //return (Node *) FunctionNode(body, params);
//        }
//    }
//
//    return nullptr;
//}

/*
 *   primary_expr
 * | primary_expr call_expr
 */

Node *RayParser::slot_access_expr() {
    Node *node;
//    long lineno = this->lex->lineno;
//    long offset = this->lex->offset;
    
    debug("slot_access_expr");
    
    
    // primary_expr
    node = this->primary_expr();
    if (!node) return nullptr;
    
    // id*
    // TODO: replace
    // while (is(ID)) {
    //   Node *right = this->call_expr();
    //   if (!right) return nullptr;
    //   node = (Node *) ray_slot_node_new(node, right);
    // }
    
    return node;
}

/*
 * (expr (',' expr)*)
 */

ArgsNode *RayParser::call_args() {
    Node *node;
    ArgsNode *args = new ArgsNode();
    
    this->in_args++;
    
    debug("args");
    do {
        if ((node = this->expr())) {
            // TODO: assert string or id
            if (accept(COLON)) {
                Node *val = this->expr();
                std::string *str = ((IDNode *)node)->val;
                (*args->hash)[*str] = val;
            }
            args->vec->push_back(node);
        } else {
            return nullptr;
        }
    } while (accept(COMMA));
    
    this->in_args--;
    
    return args;
}

/*
 *   slot_access_expr '(' args? ')'
 * | slot_access_expr '.' call_expr
 * | slot_access_expr '?.' call_expr
 * | slot_access_expr
 */

Node *RayParser::call_expr() {
    Node *node;
    CallNode *call = nullptr;
    debug("call_expr");
    
    // slot_access_expr
    if (!(node = this->slot_access_expr())) return nullptr;
    
    // '('
    if (accept(LPAREN)) {
        context("function call");
        call = new CallNode(node);
        
        // args? ')'
        if (!accept(RPAREN)) {
            call->args = this->call_args();
            if (!accept(RPAREN)) return error("missing closing ')'");
        }
        
        node = (Node *) call;
    }
    
    // '.' call_expr
    if (accept(OP_DOT)) {
        // TODO: verify slot access or call
        Node *expr = this->call_expr();
        
        if (RAY_NODE_CALL == expr->type) {
            CallNode *call = (CallNode *) expr;
            call->args->vec->push_back(node);
            node = (Node *) call;
        } else {
            node = (Node *) new SlotNode(node, expr);
        }
    }
    
    return node;
}

/*
 *   logical_or_expr
 * | 'var' call_expr '=' not_expr
 * | call_expr '=' not_expr
 * | call_expr '+=' not_expr
 * | call_expr '-=' not_expr
 * | call_expr '/=' not_expr
 * | call_expr '*=' not_expr
 * | call_expr '||=' not_expr
 */

Node *RayParser::assignment_expr() {
    RayTokenType op;
    Node *node, *right;
    int var = 0, val = 0;
    
    // var?
    if (accept(VAR)) var = 1;
    
    debug("assignment_expr");
    if (!(node = this->logical_or_expr())) return nullptr;
    
    // =
    if (accept(OP_ASSIGN)) {
        op = prev->type;
        context("assignment");
        if (!(right = this->not_expr())) return nullptr;
        BinaryOpNode *ret = new BinaryOpNode(op, node, right);
        ret->var = var;
        val = 1;
        return (Node *) ret;
    }
    
    // compound
    if ( accept(OP_PLUS_ASSIGN)
        || accept(OP_MINUS_ASSIGN)
        || accept(OP_DIV_ASSIGN)
        || accept(OP_MUL_ASSIGN)
        || accept(OP_OR_ASSIGN)
        || accept(OP_AND_ASSIGN)) {
        op = prev->type;
        context("compoound assignment");
        if (!(right = this->not_expr())) return nullptr;
        val = 1;
        return (Node *) new BinaryOpNode(op, node, right);
    }
    
    if (var && !val) return error("variable must be initialized");
    
    return node;
}

/*
 *   '!' not_expr
 * | assignment_expr
 */

Node *RayParser::not_expr() {
    debug("not_expr");
    if (accept(OP_NOT)) {
        Node *expr;
        if (!(expr = this->not_expr())) return nullptr;
        return (Node *) new UnaryOpNode(RAY_TOKEN_OP_NOT, expr, false);
    }
    return this->assignment_expr();
}

/*
 *  not_expr
 */

Node *RayParser::expr() {
    Node *node;
    debug("expr");
    if (!(node = this->not_expr())) return nullptr;
    return node;
}

/*
 * expr
 */

Node *RayParser::expr_stmt() {
    Node *node;
    debug("expr_stmt");
    
    if (!(node = this->expr())) return nullptr;
    
//    if (!accept(SEMICOLON)) {
//        return this->error("missing ';'");
//    }
    
    return node;
}

/*
 * 'type' id (id ':' id)*
 */

//Node *
//type_stmt() {
//    debug("type_stmt");
//    context("type statement");
//    ray_type_node_t *type;
//
//    // 'type'
//    if (!accept(TYPE)) return nullptr;
//
//    // id
//    if (!is(ID)) return error("missing type name");
//    const char *name = next->value.as_string;
//    type = new TypeNode(name);
//
//    // type fields
//    do {
//        // id
//        if (!(is(ID))) return error("expecting field");
//        const char *name = next->value.as_string;
//
//        // ':'
//        if (!accept(COLON)) return error("expecting ':'");
//
//        // id
//        if (!(is(ID))) return error("expecting field type");
//        const char *type = next->value.as_string;
//    } while (!accept(END));
//
//    return (Node *) type;
//}


/*
 * 'fn' id? '(' args? ')' ('->' id)? block
 */

Node *RayParser::function_stmt() {
    BlockNode *body;
    std::vector<Node*> *params;
    Node *type = nullptr;
    debug("function_stmt");
    context("function statement");
    
    // 'fn'
    if (!accept(FN)) return nullptr;
    
    // id
    std::string name;
    if (is(ID)) name = next->value;
    else name = "<anonymous>";
    
    // '('
    if (accept(LPAREN)) {
        // params?
        if (!(params = this->function_params())) params = new std::vector<Node*>();
        
        // ')'
        context("function");
        if (!accept(RPAREN)) return this->error("missing closing ')'");
    } else {
        return error("missing '('");
    }
    
    context("function");
    
    // ('->' type_expr)?
    if (accept(ARROW)) {
        type = this->type_expr();
        if (!type) return this->error("missing return type");
    }
    
    // Must not have a return type (void)
    if (!type) type = (Node *)new TypeNode(new std::vector<Node*>());
    
    // block
    if (!(body = this->block())) return error("missing function body");
    
    return (Node *) new FunctionNode(name, type, body, params);
}

/*
 * 'extern' id? '(' args? ')' ('->' id)?
 */

Node *RayParser::extern_stmt() {
    BlockNode *body;
    std::vector<Node*> *params;
    Node *type = nullptr;
    debug("extern_stmt");
    context("extern statement");
    
    // 'extern'
    if (!accept(EXTERN)) return nullptr;
    
    // id
    std::string name;
    if (is(ID)) name = next->value;
    else name = "<anonymous>";
    
    // '('
    if (accept(LPAREN)) {
        // params?
        if (!(params = this->function_params())) params = new std::vector<Node*>();
        
        // ')'
        context("extern");
        if (!accept(RPAREN)) return this->error("missing closing ')'");
    } else {
        return error("missing '('");
    }
    
    context("extern");
    
    // ('->' type_expr)?
    if (accept(ARROW)) {
        type = this->type_expr();
        if (!type) return this->error("missing return type");
    }
    
    // Must not have a return type (void)
    if (!type) type = (Node *)new TypeNode(new std::vector<Node*>());
    
    return (Node *) new ExternNode(name, type, params);
}

/*
 *  ('if' | 'unless') expr block
 *  ('else' 'if' block)*
 *  ('else' block)?
 */

/*  ('if' | 'unless') expr block
 *  ('else' ( if_stmt | block )) ?
 */
 

Node *RayParser::if_stmt() {
    Node *cond;
    BlockNode *body;
    debug("if_stmt");
    
    // ('if' | 'unless')
    if (!(accept(IF) || accept(UNLESS))) return nullptr;
    bool negate = RAY_TOKEN_UNLESS == prev->type;
    
    // expr
    context("if statement condition");
    if (!(cond = this->expr())) return nullptr;
    
    // block
    context("if statement");
    if (!(body = this->block())) return nullptr;
    
    IfNode *node = new IfNode(negate, cond, body);
    
    // 'else'
    context("else statement");
    if (accept(ELSE)) {
        // if_stmt
        if (is(IF) || is(UNLESS)) {
            node->elseBlock = this->if_stmt();
        // block
        } else {
            node->elseBlock = this->block();
        }
    }
    
//    // 'else'
//    while (true) {
//        if (accept(ELSE)) {
//            BlockNode *body;
//            
//            // ('else' 'if' block)*
//            if (accept(IF)) {
//                context("else if statement condition");
//                if (!(cond = this->expr())) return nullptr;
//                context("else if statement");
//                if (!(body = this->block())) return nullptr;
//                node->elseIfs->push_back((Node*)new IfNode(false, cond, body));
//            
//            // 'else'
//            } else {
//                context("else statement");
//                if (!(body = this->block())) return nullptr;
//                node->elseBlock = body;
//            }
//        } else {
//            break;
//        }
//    }
    
    return (Node *) node;
}

/*
 * ('while' | 'until' | 'loop') expr block
 */

Node *RayParser::while_stmt() {
    Node *cond;
    BlockNode *body;
    debug("while_stmt");
    int loop = 0;
    
    if (is(LOOP)) loop = 1;
    
    // ('loop' | 'until' | 'while')
    if (!(accept(UNTIL) || accept(WHILE) || accept(LOOP))) return nullptr;
    int negate = RAY_TOKEN_UNTIL == prev->type;
    context("while statement condition");
    
    // expr
    if (!(cond = this->expr())) {
        if (!loop) return nullptr;
        cond = (Node *)new IntNode(1);
    }
    context("while statement");
    
    // block
    if (!(body = this->block())) return nullptr;
    
    return (Node *) new WhileNode(negate, cond, body);
}

/*
 *   'return' expr
 * | 'return'
 */

Node *RayParser::return_stmt() {
    debug("return");
    context("return statement");
    
    // 'return'
    if (!accept(RETURN)) return nullptr;
    
    // expr
    Node *node;
    if (!(node = this->expr())) return (Node*)new ReturnNode(new IDNode("void"));
    
    return (Node *) new ReturnNode(node);
}

/*
 *   if_stmt
 * | while_stmt
 * | return_stmt
 * | function_stmt
 * | expr_stmt
 */

Node *RayParser::stmt() {
    debug("stmt");
    context("statement");
    if (is(IF) || is(UNLESS)) return this->if_stmt();
    if (is(WHILE) || is(UNTIL)) return this->while_stmt();
    if (is(RETURN)) return this->return_stmt();
    if (is(FN)) return this->function_stmt();
    return this->expr_stmt();
}

/*
 * '{' stmt* '}'
 */

BlockNode *RayParser::block() {
    debug("block");
    Node *node;
    BlockNode *block = new BlockNode();
    
    if (!accept(LBRACE)) return error("missing '{'");
    
    while (!accept(RBRACE)) {
        if (!(node = this->stmt())) return nullptr;
        block->stmts->push_back(node);
    }
    
    return block;
}

/*
 * stmt*
 */

BlockNode *RayParser::program() {
    debug("program");
    Node *node;
    BlockNode *block = new BlockNode();
    
    while (!accept(EOS)) {
        if ((node = this->stmt())) {
            block->stmts->push_back(node);
        } else {
            return nullptr;
        }
    }
    
    return block;
}

BlockNode *RayParser::parse() {
    return this->program();
}