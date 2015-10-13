//
//  AST.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 11/1/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include "AST.h"

Node::Node(long lineno, long offset) {
    this->lineno = lineno;
    this->offset = offset;
}

BlockNode::BlockNode() : Node(0, 0) {
    this->type = RAY_NODE_BLOCK;
    this->stmts = new std::vector<Node*>();
}

IDNode::IDNode(std::string val) : Node(0, 0) {
    this->type = RAY_NODE_ID;
    this->val = new std::string(val);
}

IntNode::IntNode(long val) : Node(0, 0) {
    this->type = RAY_NODE_INT;
    this->val = val;
}

BoolNode::BoolNode(bool val) : Node(0, 0) {
    this->type = RAY_NODE_BOOL;
    this->val = val;
}

FloatNode::FloatNode(double val) : Node(0, 0) {
    this->type = RAY_NODE_FLOAT;
    this->val = val;
}

CharNode::CharNode(char val) : Node(0, 0) {
    this->type = RAY_NODE_CHAR;
    this->val = val;
}

StringNode::StringNode(std::string *val) : Node(0, 0) {
    this->type = RAY_NODE_STRING;
    this->val = val;
}

HashNode::HashNode() : Node(0, 0) {
    this->type = RAY_NODE_HASH;
    this->vals = new std::map<std::string, Node*>();
}

ArrayNode::ArrayNode() : Node(0, 0) {
    this->type = RAY_NODE_ARRAY;
    this->vals = new std::vector<Node*>();
}

BinaryOpNode::BinaryOpNode(RayTokenType op, Node *left, Node *right, bool var) : Node(0, 0) {
    this->type = RAY_NODE_BINARY_OP;
    this->op = op;
    this->left = left;
    this->right = right;
    this->var = var;
}

UnaryOpNode::UnaryOpNode(RayTokenType op, Node *expr, bool postfix) : Node(0, 0) {
    this->type = RAY_NODE_UNARY_OP;
    this->op = op;
    this->expr = expr;
    this->postfix = postfix;
}

CallNode::CallNode(Node *expr) : Node(0, 0) {
    this->type = RAY_NODE_CALL;
    this->expr = expr;
    this->args = new ArgsNode();
}

SlotNode::SlotNode(Node *left, Node *right) : Node(0, 0) {
    this->type = RAY_NODE_SLOT;
    this->left = left;
    this->right = right;
}

ArgsNode::ArgsNode() : Node(0, 0) {
    this->type = RAY_NODE_ARGS;
    this->vec = new std::vector<Node*>();
    this->hash = new std::map<std::string, Node*>();
}

ReturnNode::ReturnNode(Node *expr) : Node(0, 0) {
    this->type = RAY_NODE_RETURN;
    this->expr = expr;
}

WhileNode::WhileNode(bool negate, Node *cond, BlockNode *block) : Node(0, 0) {
    this->type = RAY_NODE_WHILE;
    this->negate = negate;
    this->cond = cond;
    this->block = block;
}

IfNode::IfNode(bool negate, Node *cond, BlockNode *block) : Node(0, 0) {
    this->type = RAY_NODE_IF;
    this->negate = negate;
    this->cond = cond;
    this->block = block;
}

TypeNode::TypeNode(std::vector<Node*> *types) : Node(0, 0) {
    this->type = RAY_NODE_TYPE;
    this->types = types;
}

ExternNode::ExternNode(std::string name, Node *fntype, std::vector<Node*> *params) : Node(0, 0) {
    this->type = RAY_NODE_EXTERN;
    this->name = name;
    this->fntype = fntype;
    this->params = params;
}

FunctionNode::FunctionNode(std::string name, Node *fntype, BlockNode *block, std::vector<Node*> *params) : Node(0, 0) {
    this->type = RAY_NODE_FUNCTION;
    this->name = name;
    this->fntype = fntype;
    this->block = block;
    this->params = params;
}

DeclNode::DeclNode(std::string name, Node *vartype, Node *val=nullptr) : Node(0, 0) {
    this->type = RAY_NODE_DECL;
    this->name = name;
    this->vartype = vartype;
    this->val = val;
}

SequenceNode::SequenceNode(std::vector<Node*> *nodes) : Node(0, 0) {
    this->nodes = nodes;
}