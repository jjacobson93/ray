//
//  Visitor.h
//  rayc
//
//  Created by Jeremy Jacobson on 11/1/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__Visitor__
#define __rayc__Visitor__

#include "AST.h"

/*
 * Visitor class.
 */

class RayVisitor {
    
public:
    void *visit(Node *node);
    void *visitBlock(BlockNode *node) { return nullptr; }
    void *visitID(IDNode *node) { return nullptr; }
    void *visitInt(IntNode *node) { return nullptr; }
    void *visitFloat(FloatNode *node) { return nullptr; }
    void *visitString(StringNode *node) { return nullptr; }
    void *visitChar(CharNode *node) { return nullptr; }
    void *visitBool(BoolNode *node) { return nullptr; }
    void *visitType(TypeNode *node) { return nullptr; }
    void *visitSlot(SlotNode *node) { return nullptr; }
    void *visitCall(CallNode *node) { return nullptr; }
    void *visitWhile(WhileNode *node) { return nullptr; }
    void *visitUnaryOp(UnaryOpNode *node) { return nullptr; }
    void *visitBinaryOp(BinaryOpNode *node) { return nullptr; }
    void *visitFunction(FunctionNode *node) { return nullptr; }
    void *visitExtern(ExternNode *node) { return nullptr; }
    void *visitArray(ArrayNode *node) { return nullptr; }
    void *visitHash(HashNode *node) { return nullptr; }
    void *visitReturn(ReturnNode *node) { return nullptr; }
    void *visitDecl(DeclNode *node) { return nullptr; }
    void *visitIf(IfNode *node) { return nullptr; }
    void *visitSequence(SequenceNode *node) { return nullptr; }
};

#endif /* defined(__rayc__Visitor__) */
