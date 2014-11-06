//
//  Codegen.h
//  rayc
//
//  Created by Jeremy Jacobson on 11/1/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__Codegen__
#define __rayc__Codegen__

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/PassManager.h>
#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Analysis/Lint.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/Triple.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/Timer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/Memory.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Instrumentation.h>
#include <llvm/Transforms/Vectorize.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Linker/Linker.h>

#include "Visitor.h"

class RayCodegen {
    
public:
    RayCodegen();
    void gen(Node *root);
};

class RayModuleContext {
    
public:
    llvm::Module *module;
    
    RayModuleContext(llvm::Module *module);
    llvm::IRBuilder<> builder();
};

class RayFunctionContext {
    
public:
    RayModuleContext *mcx;
    llvm::Value *retAlloca;
    llvm::BasicBlock *retBlock;
    
    RayFunctionContext(RayModuleContext *mcx);
};

class RayCodegenScope : public std::map<std::string, llvm::Value*> {

public:
    RayCodegenScope *parentScope;
    
    RayCodegenScope();
    RayCodegenScope(RayCodegenScope *parentScope);
};

class RayCodegenContext {
    
public:
    llvm::BasicBlock *bb;
    RayCodegenScope *scope;
    RayFunctionContext *fcx;
    
//    RayCodegenContext(RayCodegenContext *ctx);
    RayCodegenContext(llvm::BasicBlock *bb, RayCodegenScope *scope, RayFunctionContext *fcx);
    llvm::Function *getFunction(std::string fname);
    RayCodegenScope *popOffScope();
};

class RayCodegenVisitor : public RayVisitor {

public:
    RayCodegenVisitor() : RayVisitor() {}
    void *visit(Node *node, RayCodegenContext *ctx);
    void *visitBlock(BlockNode *node, RayCodegenContext *ctx);
    void *visitID(IDNode *node, RayCodegenContext *ctx);
    void *visitInt(IntNode *node, RayCodegenContext *ctx);
    void *visitFloat(FloatNode *node, RayCodegenContext *ctx);
    void *visitString(StringNode *node, RayCodegenContext *ctx);
    void *visitChar(CharNode *node, RayCodegenContext *ctx);
    void *visitBool(BoolNode *node, RayCodegenContext *ctx);
    void *visitType(TypeNode *node, RayCodegenContext *ctx);
    void *visitSlot(SlotNode *node, RayCodegenContext *ctx);
    void *visitCall(CallNode *node, RayCodegenContext *ctx);
    void *visitWhile(WhileNode *node, RayCodegenContext *ctx);
    void *visitUnaryOp(UnaryOpNode *node, RayCodegenContext *ctx);
    void *visitBinaryOp(BinaryOpNode *node, RayCodegenContext *ctx);
    void *visitFunction(FunctionNode *node, RayCodegenContext *ctx);
    void *visitArray(ArrayNode *node, RayCodegenContext *ctx);
    void *visitHash(HashNode *node, RayCodegenContext *ctx);
    void *visitReturn(ReturnNode *node, RayCodegenContext *ctx);
    void *visitDecl(DeclNode *node, RayCodegenContext *ctx);
    void *visitIf(IfNode *node, RayCodegenContext *ctx);
};

#endif /* defined(__rayc__Codegen__) */
