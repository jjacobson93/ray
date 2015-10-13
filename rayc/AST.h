//
//  AST.h
//  rayc
//
//  Created by Jeremy Jacobson on 11/1/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__AST__
#define __rayc__AST__

#include <string>
#include <vector>
#include <map>
#include "Token.h"

typedef enum {
    RAY_NODE_BLOCK,
    RAY_NODE_EXPR_STMT,
    RAY_NODE_RETURN,
    RAY_NODE_FN,
    RAY_NODE_IF,
    RAY_NODE_WHILE,
    RAY_NODE_FOR,
    RAY_NODE_UNARY_OP,
    RAY_NODE_BINARY_OP,
    RAY_NODE_TERNARY_OP,
    RAY_NODE_BOOL,
    RAY_NODE_NULL,
    RAY_NODE_ID,
    RAY_NODE_DECL,
    RAY_NODE_CALL,
    RAY_NODE_ARGS,
    RAY_NODE_INT,
    RAY_NODE_FLOAT,
    RAY_NODE_STRING,
    RAY_NODE_CHAR,
    RAY_NODE_ARRAY,
    RAY_NODE_HASH,
    RAY_NODE_FUNCTION,
    RAY_NODE_EXTERN,
    RAY_NODE_TYPE,
    RAY_NODE_SLOT,
    RAY_NODE_SEQUENCE
} RayNodeType;

class Node {
    
public:
    RayNodeType type;
    long lineno;
    long offset;
    
    Node(long lineno, long offset);
};

class BlockNode : public Node {
    
public:
    std::vector<Node*> *stmts;
    
    BlockNode();
};

class ArrayNode : public Node {
    
public:
    std::vector<Node*> *vals;
    ArrayNode();
};

class HashNode : public Node {
    
public:
    std::map<std::string, Node*> *vals;
    HashNode();
};

class ArgsNode : public Node {
    
public:
    std::vector<Node*> *vec;
    std::map<std::string, Node*> *hash;
    ArgsNode();
};

class IDNode : public Node {
    
public:
    std::string *val;
    
    IDNode(std::string val);
};

class IntNode : public Node {
    
public:
    long val;
    
    IntNode(long val);
};

class FloatNode : public Node {
    
public:
    double val;
    
    FloatNode(double val);
};

class BoolNode : public Node {
    
public:
    bool val;
    
    BoolNode(bool val);
};

class CharNode : public Node {
    
public:
    char val;
    
    CharNode(char val);
};

class StringNode : public Node {
    
public:
    std::string *val;
    
    StringNode(std::string *val);
};

class BinaryOpNode : public Node {
    
public:
    RayTokenType op;
    Node *left;
    Node *right;
    bool var;
    
    BinaryOpNode(RayTokenType op, Node *left, Node *right, bool var=false);
};

class UnaryOpNode : public Node {
    
public:
    RayTokenType op;
    Node *expr;
    bool postfix;
    
    UnaryOpNode(RayTokenType op, Node *expr, bool postfix);
};

class ReturnNode : public Node {
    
public:
    Node *expr;
    
    ReturnNode(Node *expr);
};

class WhileNode : public Node {
    
public:
    bool negate;
    Node *cond;
    BlockNode *block;
    
    WhileNode(bool negate, Node *cond, BlockNode *block);
};

class IfNode : public Node {
    
public:
    bool negate;
    Node *cond;
    BlockNode *block;
    std::vector<Node*> *elseIfs;
    Node *elseBlock;
    
    IfNode(bool negate, Node *cond, BlockNode *block);
};

class TypeNode : public Node {
    
public:
    std::vector<Node*> *types;
    
    TypeNode(std::vector<Node*> *types);
};

class ExternNode : public Node {
    
public:
    std::string name;
    Node *fntype;
    std::vector<Node*> *params;
    
    ExternNode(std::string name, Node *fntype, std::vector<Node*> *params);
};

class FunctionNode : public Node {
    
public:
    std::string name;
    Node *fntype;
    BlockNode *block;
    std::vector<Node*> *params;
    
    FunctionNode(std::string name, Node *fntype, BlockNode *block, std::vector<Node*> *params);
};

class CallNode : public Node {
    
public:
    Node *expr;
    ArgsNode *args;
    
    CallNode(Node *expr);
};

class SlotNode : public Node {
    
public:
    Node *left;
    Node *right;
    
    SlotNode(Node *left, Node *right);
};

class DeclNode : public Node {
    
public:
    std::string name;
    Node *vartype;
    Node *val;
    
    DeclNode(std::string name, Node *vartype, Node *val);
};

class SequenceNode : public Node {
    
public:
    std::vector<Node*> *nodes;
    
    SequenceNode(std::vector<Node*> *nodes);
};

#endif /* defined(__rayc__AST__) */
