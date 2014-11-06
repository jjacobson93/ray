//
//  Visitor.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 11/1/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include "Visitor.h"

/*
 * Visit the node when a `type` callback is present.
 */

#define VISIT(type) \
return this->visit##type((type##Node *) node);

/*
 * Visit `node`, invoking the associated callback.
 */

void *RayVisitor::visit(Node *node) {
    switch (node->type) {
        case RAY_NODE_BLOCK: VISIT(Block);
        case RAY_NODE_ID: VISIT(ID);
        case RAY_NODE_DECL: VISIT(Decl);
        case RAY_NODE_INT: VISIT(Int);
        case RAY_NODE_FLOAT: VISIT(Float);
        case RAY_NODE_STRING: VISIT(String);
        case RAY_NODE_CHAR: VISIT(Char);
        case RAY_NODE_BOOL: VISIT(Bool);
        case RAY_NODE_TYPE: VISIT(Type);
        case RAY_NODE_SLOT: VISIT(Slot);
        case RAY_NODE_CALL: VISIT(Call);
        case RAY_NODE_IF: VISIT(If);
        case RAY_NODE_WHILE: VISIT(While);
        case RAY_NODE_UNARY_OP: VISIT(UnaryOp);
        case RAY_NODE_BINARY_OP: VISIT(BinaryOp);
        case RAY_NODE_FUNCTION: VISIT(Function);
        case RAY_NODE_ARRAY: VISIT(Array);
        case RAY_NODE_HASH: VISIT(Hash);
        case RAY_NODE_RETURN: VISIT(Return);
        default: return NULL;
    }
}