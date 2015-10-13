//
//  Codegen.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 11/1/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include "Codegen.h"
#include "JSCodegen.h"
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <map>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/Utils.h>
#include <clang/Lex/HeaderSearch.h>
#include <clang/Lex/HeaderSearchOptions.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/TargetInfo.h>
//#include <clang-c/Index.h>

/*
 * Visit the node when a `type` callback is present.
 */

#define VISIT(type) \
return this->visit##type((type##Node *) node, ctx);

#define i1 llvm::IntegerType::getInt1Ty(llvm::getGlobalContext())
#define i8 llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())
#define i32 llvm::IntegerType::getInt32Ty(llvm::getGlobalContext())
#define f64 llvm::IntegerType::getDoubleTy(llvm::getGlobalContext())
#define StrTy llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext())
#define Array(ety, size) llvm::ArrayType(ety, size)
#define VoidTy llvm::Type::getVoidTy(llvm::getGlobalContext())

//static llvm::Module *module;
//static llvm::IRBuilder<> builder(llvm::getGlobalContext());
//static std::map<std::string, llvm::AllocaInst*> namedValues;

typedef enum {
    RAY_TYPE_ID,
    RAY_TYPE_INST
} RayValueType;

typedef struct ValueWithType {
    void *value;
    llvm::Type *type;
} ValueWithType;

void *error(std::string str) {
    std::cerr << str << std::endl;
    exit(1);
    return NULL;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
//static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, llvm::Type *type, const std::string &VarName) {
//    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().end());
//    return TmpB.CreateAlloca(type, 0, VarName.c_str());
//}

llvm::IRBuilder<> Build(RayCodegenContext *ctx) {
    llvm::IRBuilder<> builder = ctx->fcx->mcx->builder();
    builder.SetInsertPoint(ctx->bb);
    return builder;
}

/***************************/
/****** CONTEXT CLASS ******/
/***************************/

RayCodegenContext::RayCodegenContext(llvm::BasicBlock *bb, RayCodegenScope *scope, RayFunctionContext *fcx) {
    this->bb = bb;
    this->scope = new RayCodegenScope(scope);
    this->fcx = fcx;
}

llvm::Function *RayCodegenContext::getFunction(std::string fname) {
    return this->fcx->mcx->module->getFunction(fname);
}

RayCodegenScope *RayCodegenContext::popOffScope() {
    RayCodegenScope *parentScope = this->scope->parentScope;
    delete this->scope;
    this->scope = parentScope;
    return parentScope;
}

RayFunctionContext::RayFunctionContext(RayModuleContext *mcx) {
    this->mcx = mcx;
}

RayModuleContext::RayModuleContext(llvm::Module *module) {
    this->module = module;
}

llvm::IRBuilder<> RayModuleContext::builder() {
    return llvm::IRBuilder<>(this->module->getContext());
}

RayCodegenScope::RayCodegenScope() : std::map<std::string, llvm::Value *>() {
    this->parentScope = nullptr;
}

RayCodegenScope::RayCodegenScope(RayCodegenScope *parentScope) : std::map<std::string, llvm::Value *>(*parentScope) {
    this->parentScope = parentScope;
}

/***************************/
/****** CODEGEN CLASS ******/
/***************************/

RayCodegen::RayCodegen() {
    
}

void* RayCodegen::gen(Node *root, RayTarget target) {
    RayCodegenVisitor *visitor;
    if (target == RAY_TARGET_NATIVE)
        visitor = new RayCodegenVisitor();
    else
        return nullptr;
//        visitor = new RayJSCodegenVisitor();
    llvm::Module *module = new llvm::Module("test.ray", llvm::getGlobalContext());
    RayModuleContext *mctx = new RayModuleContext(module);
    RayCodegenContext *gctx = new RayCodegenContext(nullptr,
                                                    new RayCodegenScope(),
                                                    new RayFunctionContext(mctx));
    this->builtins(gctx);
    visitor->visit(root, gctx);
    
    module->dump();

    return module;
}


// Builtins

//void stdlibPowerInt(RayCodegenContext *ctx) {
//    llvm::Module *mod = ctx->fcx->mcx->module;
//    llvm::Type* paramTypes[] = { i32, i32 };
//    std::string paramNames[] = { "base", "power" };
//    
//    llvm::FunctionType *fnType = llvm::FunctionType::get(i32, paramTypes, false);
//    llvm::Function *func = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "@__RAY_STDLIB_POW_INT", mod);
//    
//    int idx = 0;
//    llvm::Function::arg_iterator ai;
//    for (ai = func->arg_begin(); ai != func->arg_end(); ++ai, ++idx) {
//        ai->setName(paramNames[idx]);
//    }
//    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
//    llvm::BasicBlock *ifBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.body", func);
//    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.else", func);
//    llvm::BasicBlock *retBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", func);
//    
//    ctx->bb = entryBlock;
//
//    ai = func->arg_begin();
//    llvm::Value *base = ai;
//    ++ai;
//    llvm::Value *power = ai;
//    
//    llvm::Value *retval = Build(ctx).CreateAlloca(i32, 0, "retval");
//    llvm::Value *condval = Build(ctx).CreateICmpEQ(power, llvm::ConstantInt::get(i32, 0), "cond");
//    
//    Build(ctx).CreateCondBr(condval, ifBlock, elseBlock);
//    
//    ctx->bb = ifBlock;
//    
//    Build(ctx).CreateStore(llvm::ConstantInt::get(i32, 1), retval);
//    Build(ctx).CreateBr(retBlock);
//    
//    ctx->bb = elseBlock;
//    
//    llvm::Value *dec = Build(ctx).CreateAdd(power, <#llvm::Value *RHS#>)
//    llvm::Value *rec = Build(ctx).CreateCall(func, { base, )
//}

void RayCodegen::builtins(RayCodegenContext *ctx) {
    llvm::Module *mod = ctx->fcx->mcx->module;
    llvm::Function::Create(llvm::FunctionType::get(i32, { i32, i32 }, false), llvm::GlobalValue::ExternalLinkage, "__RAY_RUNTIME_POW_INT", mod);
    llvm::Function::Create(llvm::FunctionType::get(f64, { f64, f64 }, false), llvm::GlobalValue::ExternalLinkage, "__RAY_RUNTIME_POW_FLOAT", mod);
    
    llvm::Function::Create(llvm::FunctionType::get(i32, { StrTy } , true), llvm::GlobalValue::ExternalLinkage, "printf", mod);
    
    llvm::Function::Create(llvm::FunctionType::get(i32, { StrTy } , true), llvm::GlobalValue::ExternalLinkage, "strlen", mod);
//    llvm::Function::Create(llvm::FunctionType::get(llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()), {llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()) } , false), llvm::GlobalValue::ExternalLinkage, "print", mod);
//    llvm::Function::Create(llvm::FunctionType::get(VoidTy, { StrTy } , false), llvm::GlobalValue::ExternalLinkage, "print", mod);
    
//    FILE *proc = popen("clang ray_builtins.c -S -emit-llvm -o -", "r");
//    char buffer[255];
//    std::string stdout;
//    while ( fgets(buffer, 255, proc) != NULL )
//        stdout.append(buffer);
//    pclose(proc);
//    std::cout << std::endl << "output: " << std::endl << stdout << std::endl;
    
//    clang::DiagnosticOptions diagnosticOptions;
//    clang::TextDiagnosticPrinter tdp( llvm::outs(), &diagnosticOptions );
//    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagIDs( new clang::DiagnosticIDs );
//    clang::DiagnosticsEngine diagEngine(diagIDs, &diagnosticOptions);
//    clang::Diagnostic diag(&diagEngine);
//    
//    clang::FileSystemOptions    fsOptions;
//    clang::FileManager fm( fsOptions );
//    
//    clang::SourceManager sm( diagEngine, fm );
//    llvm::IntrusiveRefCntPtr< clang::HeaderSearchOptions > hsopts;
//    clang::LangOptions langOpts;
//    
//    clang::TargetOptions *targetOpts = new clang::TargetOptions();
////    targetOpts.Triple = llvm::get;
//    clang::TargetInfo* ti = clang::TargetInfo::CreateTargetInfo( diagEngine, (const std::shared_ptr<clang::TargetOptions>)targetOpts);
//    
//    clang::HeaderSearch hs( hsopts, sm, diagEngine, langOpts, ti );
//    
//    clang::HeaderSearchOptions  headerSearchOptions;
//    clang::ApplyHeaderSearchOptions( hs, headerSearchOptions, langOpts, ti->getTriple() );
//    
//    clang::PreprocessorOptions ppo;
//    clang::ModuleLoader loader;
//    clang::Preprocessor pp( ppo, &diagEngine, &langOpts, &sm, &hs, &loader);
//    
//    clang::FrontendOptions frontendOptions;
//    clang::InitializePreprocessor( pp, ppo, headerSearchOptions, frontendOptions );
//    
//    pp.getBuiltinInfo().InitializeBuiltins( pp.getIdentifierTable(), langOpts );
//    
//
//    std::ifstream infile("ray_builtins.c");
//    std::string str((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
//    llvm::MemoryBuffer* sourceBuffer = llvm::MemoryBuffer::getMemBufferCopy( str );
//    sm.
//    sm.createMainFileIDForMemBuffer( sourceBuffer );
//    
//    clang::Builtin::Context bic( *ti );
//    clang::ASTContext astc( langOptions, sm, *ti,
//                           pp.getIdentifierTable(),
//                           pp.getSelectorTable(),
//                           bic,
//                           0 );
//    
//    llvm::LLVMContext   lc;
//    clang::CodeGenOptions codeGenOptions;
//    llvm::OwningPtr<clang::CodeGenerator> cg;
//    cg.reset( clang::CreateLLVMCodeGen( diag, "clang_test", codeGenOptions, lc ) );
//    if( cg == NULL ) {
//        printf( "could not create CodeGenerator\n" );
//        return -1;
//    }
//    
//    clang::ParseAST( pp, cg.get(), astc );
//    if( tdp.getNumErrors() ) {
//        printf( "error parsing AST\n" );
//        return -2;
//    }
//    
//    llvm::Module* new_module = cg->ReleaseModule();
    
    
//    CXIndex idx = clang_createIndex(0, 1);
    
//    char *args[] = { "-S", "-emit-llvm" };
//    CXTranslationUnit tu = clang_createTranslationUnitFromSourceFile(idx, "ray_builtins.c", 2, args, 0, 0);
//    clang_visi
//    stdlibPowerInt(mod);
}



/***************************/
/****** VISITOR CLASS ******/
/***************************/

/*
 * Visit `node`, invoking the associated callback.
 */
void *RayCodegenVisitor::visit(Node *node, RayCodegenContext *ctx) {
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
        case RAY_NODE_SEQUENCE: VISIT(Sequence);
        default: return NULL;
    }
}

void *RayCodegenVisitor::visitBlock(BlockNode *node, RayCodegenContext *ctx) {
    void *retVal = nullptr;
    
    RayCodegenScope *blockScope = new RayCodegenScope(ctx->scope);
    ctx->scope = blockScope;
    for (std::vector<Node*>::iterator it = node->stmts->begin(); it != node->stmts->end(); ++it) {
        retVal = this->visit(*it, ctx);
    }
    ctx->popOffScope();
    return retVal;
}

void *RayCodegenVisitor::visitID(IDNode *node, RayCodegenContext *ctx) {
//    if (std::find(keywords, keywords + 16, node->val) != keywords + 16) {
//        return new std::string(node->val);
//    }
//    
//    // Look up variable
//    llvm::Value *val = namedValues[node->val];
//    if (!val) return new std::string(node->val);
//    
//    // Load value
//    return builder.CreateLoad(val, node->val.c_str());
    return node->val;
}

void *RayCodegenVisitor::visitInt(IntNode *node, RayCodegenContext *ctx) {
    return llvm::ConstantInt::get(i32, node->val);
}

void *RayCodegenVisitor::visitFloat(FloatNode *node, RayCodegenContext *ctx) {
    return llvm::ConstantFP::get(f64, node->val);
}

void *RayCodegenVisitor::visitString(StringNode *node, RayCodegenContext *ctx) {
//    return llvm::ConstantDataArray::getString(llvm::getGlobalContext(), node->val->c_str(), true);
    return Build(ctx).CreateGlobalStringPtr(node->val->c_str());
}

void *RayCodegenVisitor::visitChar(CharNode *node, RayCodegenContext *ctx) {
    return nullptr;
}

void *RayCodegenVisitor::visitBool(BoolNode *node, RayCodegenContext *ctx) {
    return llvm::ConstantInt::get(i1, node->val);
}

void *RayCodegenVisitor::visitType(TypeNode *node, RayCodegenContext *ctx) {
    for (std::vector<Node*>::iterator it = node->types->begin(); it != node->types->end(); ++it) {
        std::string *tyname = (std::string*)this->visit(*it, ctx);
        if (tyname->compare("int") == 0) {
            return i32;
        } else if (tyname->compare("float") == 0) {
            return f64;
        } else if (tyname->compare("bool") == 0) {
            return i1;
        } else if (tyname->compare("str") == 0) {
            return StrTy;
        } else {
            return VoidTy;
        }
    }
    
    return llvm::Type::getVoidTy(llvm::getGlobalContext());
}

void *RayCodegenVisitor::visitSlot(SlotNode *node, RayCodegenContext *ctx) {
    return nullptr;
}

void *RayCodegenVisitor::visitCall(CallNode *node, RayCodegenContext *ctx) {
    std::string *callee = (std::string*)this->visit(node->expr, ctx);
    llvm::Function *func = ctx->getFunction(*callee);
    if (!func) return error("unknown function: " + *callee);

    
    std::vector<llvm::Value*> args;
    if (node->args->vec->size()) {
        Node *curr;
        for (std::vector<Node*>::iterator it = node->args->vec->begin(); it != node->args->vec->end(); ++it) {
            curr = *it;
            void *arg = this->visit(*it, ctx);
            if (curr->type == RAY_NODE_ID) {
                std::string argAsString = *(std::string*)arg;
                if (ctx->scope->find(argAsString) == ctx->scope->end()) return error("Unknown variable: " + argAsString);
                
                arg = Build(ctx).CreateLoad((*ctx->scope)[argAsString]);
            }
            
            args.push_back((llvm::Value*)arg);
        }
    }
    
    return Build(ctx).CreateCall(func, args);
}

void *RayCodegenVisitor::visitWhile(WhileNode *node, RayCodegenContext *ctx) {
    bool negate = node->negate;
    
    RayCodegenScope *whileScope = new RayCodegenScope(ctx->scope);
    ctx->scope = whileScope;
    
    llvm::Function *func = Build(ctx).GetInsertBlock()->getParent();
    // creat blocks
    llvm::BasicBlock *condBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while.cond", func, ctx->fcx->retBlock);
    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while.body", func, ctx->fcx->retBlock);
    llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while.end", func, ctx->fcx->retBlock);
    
    Build(ctx).CreateBr(condBlock);
    
    ctx->bb = condBlock;
    
    void *cond = this->visit(node->cond, ctx);
    if (node->cond->type == RAY_NODE_ID) {
        cond = Build(ctx).CreateLoad((*ctx->scope)[*(std::string*)cond]);
    }
    
    llvm::Value *condValue = (llvm::Value*)cond;
    
    if (negate) {
        condValue = Build(ctx).CreateNot(condValue);
    }
    
    // create conditional break
    Build(ctx).CreateCondBr(condValue, bodyBlock, endBlock);
    
    // emit then
    ctx->bb = bodyBlock;
    
    // visit block
    this->visit(node->block, ctx);
    Build(ctx).CreateBr(condBlock);
    
    // pop off 'while' scope
    ctx->popOffScope();
    
    // set end block as context's bb
    ctx->bb = endBlock;
    
    return endBlock;
}

void *RayCodegenVisitor::visitUnaryOp(UnaryOpNode *node, RayCodegenContext *ctx) {
    void *expr = this->visit(node->expr, ctx);
    
    llvm::Value *alloca = nullptr;
    if (node->expr->type == RAY_NODE_ID) {
        std::string *name = (std::string*)expr;
        alloca = (*ctx->scope)[*name];
        if (!alloca) return error("variable '" + *name + "' is undefined");
        
        // load
        expr = Build(ctx).CreateLoad(alloca);
    }
    
    llvm::Value *exprValue = (llvm::Value*)expr;
    llvm::Type *exprType = exprValue->getType();
    
    llvm::Value *rhs;
    
    switch (node->op) {
        case RAY_TOKEN_OP_INCR:
            if (exprType == i32) {
                rhs = Build(ctx).CreateAdd(exprValue, llvm::ConstantInt::get(i32, 1));
            } else {
                rhs = Build(ctx).CreateFAdd(exprValue, llvm::ConstantFP::get(f64, 1.0));
            }
            Build(ctx).CreateStore(rhs, alloca);
            return (node->postfix) ? exprValue : rhs;
        case RAY_TOKEN_OP_DECR:
            if (exprType == i32) {
                rhs = Build(ctx).CreateSub(exprValue, llvm::ConstantInt::get(i32, 1));
            } else {
                rhs = Build(ctx).CreateFSub(exprValue, llvm::ConstantFP::get(f64, 1.0));
            }
            Build(ctx).CreateStore(rhs, alloca);
            return (node->postfix) ? exprValue : rhs;
        case RAY_TOKEN_OP_BIT_NOT:
            if (exprType == i32) {
                rhs = Build(ctx).CreateAdd(exprValue, llvm::ConstantInt::get(i32, 1));
            } else {
                return error("cannot bitwise not type");
            }
            Build(ctx).CreateStore(rhs, alloca);
            return (node->postfix) ? exprValue : rhs;
        case RAY_TOKEN_OP_PLUS:
            return exprValue;
        case RAY_TOKEN_OP_MINUS:
            if (exprType == i32) {
                rhs = Build(ctx).CreateNeg(exprValue);
            } else {
                rhs = Build(ctx).CreateFNeg(exprValue);
            }
            Build(ctx).CreateStore(rhs, alloca);
            return (node->postfix) ? exprValue : rhs;
        case RAY_TOKEN_OP_NOT:
            if (exprType == i32) {
                rhs = Build(ctx).CreateICmpNE(exprValue, llvm::ConstantInt::get(i32, 0));
            } else {
                rhs = Build(ctx).CreateFCmpONE(exprValue, llvm::ConstantFP::get(f64, 0.0));
            }
            Build(ctx).CreateStore(rhs, alloca);
            return (node->postfix) ? exprValue : rhs;
        default:
            return error("invalid operator");
    }
    
    return expr;
}

bool opIsAssignment(RayTokenType op) {
    return op == RAY_TOKEN_OP_ASSIGN ||
           op == RAY_TOKEN_OP_PLUS_ASSIGN ||
           op == RAY_TOKEN_OP_MINUS_ASSIGN ||
           op == RAY_TOKEN_OP_MUL_ASSIGN ||
           op == RAY_TOKEN_OP_DIV_ASSIGN ||
           op == RAY_TOKEN_OP_AND_ASSIGN ||
           op == RAY_TOKEN_OP_OR_ASSIGN;
}

void *RayCodegenVisitor::visitBinaryOp(BinaryOpNode *node, RayCodegenContext *ctx) {
    void *lhs = this->visit(node->left, ctx);
    void *rhs = this->visit(node->right, ctx);
    
    // if lhs is an ID, we need to load it or resolve it
    if (node->left->type == RAY_NODE_ID) {
        if (!node->var) { // not an assign; we only want to look up previously defined variables
            std::string *lname = (std::string*)lhs;
            llvm::Value *lalloca = (*ctx->scope)[*lname];
            if (!lalloca) return error("variable '" + *lname + "' is undefined");
            
            if (!opIsAssignment(node->op)) {
                // load
                lhs = Build(ctx).CreateLoad(lalloca);
            } else {
                lhs = lalloca;
            }
        }
    }
    
    // if rhs is an ID we need to load it or resolve it
    if (node->right->type == RAY_NODE_ID) {
//        if (!node->var) { // not an assign; we only want to look up previously defined variables
            std::string *rname = (std::string*)rhs;
            llvm::Value *ralloca = (*ctx->scope)[*rname];
            if (!ralloca) return error("variable '" + *rname + "' is undefined");
            
            if (!opIsAssignment(node->op)) {
                // load
                rhs = Build(ctx).CreateLoad(ralloca);
            } else {
                rhs = ralloca;
            }
//        }
    }
    
    llvm::Value *lhsValue = (llvm::Value*)lhs;
    llvm::Value *rhsValue = (llvm::Value*)rhs;
    
    llvm::Type *lhsType = lhsValue->getType();
    llvm::Type *rhsType = rhsValue->getType();
    
    
    if (lhsType && rhsType && !lhsType->isPointerTy() && !rhsType->isPointerTy() && lhsType != rhsType) {
        return error("ray currently does not support implicit casting");
    }
    
    switch (node->op) {
        case RAY_TOKEN_OP_PLUS:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateAdd(lhsValue, rhsValue);
            return Build(ctx).CreateFAdd(lhsValue, rhsValue);
        case RAY_TOKEN_OP_MINUS:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateSub(lhsValue, rhsValue);
            return Build(ctx).CreateFSub(lhsValue, rhsValue);
        case RAY_TOKEN_OP_MUL:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateMul(lhsValue, rhsValue);
            return Build(ctx).CreateFMul(lhsValue, rhsValue);
        case RAY_TOKEN_OP_DIV:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateSDiv(lhsValue, rhsValue);
            return Build(ctx).CreateFDiv(lhsValue, rhsValue);
        case RAY_TOKEN_OP_MOD:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateSRem(lhsValue, rhsValue);
            return Build(ctx).CreateFRem(lhsValue, rhsValue);
        case RAY_TOKEN_OP_POW:
            return Build(ctx).CreateCall((lhsType->isIntegerTy() ? ctx->getFunction("__RAY_RUNTIME_POW_INT") : ctx->getFunction("__RAY_RUNTIME_POW_FLOAT")), { lhsValue, rhsValue });
        case RAY_TOKEN_OP_AND:
            return Build(ctx).CreateAnd(lhsValue, rhsValue);
        case RAY_TOKEN_OP_OR:
            return Build(ctx).CreateOr(lhsValue, rhsValue);
        case RAY_TOKEN_OP_EQ:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateICmpEQ(lhsValue, rhsValue);
            return Build(ctx).CreateFCmpOEQ(lhsValue, rhsValue);
        case RAY_TOKEN_OP_NEQ:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateICmpNE(lhsValue, rhsValue);
            return Build(ctx).CreateFCmpONE(lhsValue, rhsValue);
        case RAY_TOKEN_OP_GT:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateICmpSGT(lhsValue, rhsValue);
            return Build(ctx).CreateFCmpOGT(lhsValue, rhsValue);
        case RAY_TOKEN_OP_LT:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateICmpSLT(lhsValue, rhsValue);
            return Build(ctx).CreateFCmpOLT(lhsValue, rhsValue);
        case RAY_TOKEN_OP_GTE:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateICmpSGE(lhsValue, rhsValue);
            return Build(ctx).CreateFCmpOGE(lhsValue, rhsValue);
        case RAY_TOKEN_OP_LTE:
            if (lhsType->isIntegerTy())
                return Build(ctx).CreateICmpSLE(lhsValue, rhsValue);
            return Build(ctx).CreateFCmpOLE(lhsValue, rhsValue);
        case RAY_TOKEN_OP_ASSIGN: {
            if (node->var) {
                ctx->fcx->mcx->module->dump();
                llvm::Value *alloca = Build(ctx).CreateAlloca(rhsType, 0, *(std::string*)lhs);
                Build(ctx).CreateStore(rhsValue, alloca);
                (*ctx->scope)[*(std::string*)lhs] = alloca;
            } else {
                Build(ctx).CreateStore(rhsValue, lhsValue);
            }
        }
        default:
            break;
    }
    
    return nullptr;
}

void *RayCodegenVisitor::visitFunction(FunctionNode *node, RayCodegenContext *ctx) {
    llvm::Type *rettype = (llvm::Type*)this->visit(node->fntype, ctx);
    llvm::FunctionType *fnType;
    
    std::vector<llvm::Type*> paramTypes;
    std::vector<std::string*> paramNames;
    
    RayCodegenScope *functionScope = new RayCodegenScope(ctx->scope);
    ctx->scope = functionScope;
    
    // Get params
    for (std::vector<Node*>::iterator it = node->params->begin(); it != node->params->end(); ++it) {
        ValueWithType *param = (ValueWithType*)this->visit(*it, ctx);
        paramTypes.push_back(param->type);
        paramNames.push_back((std::string*)param->value);
    }
    
    // Create function
    fnType = llvm::FunctionType::get(rettype, paramTypes, false);
    llvm::Function *func = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, node->name, ctx->fcx->mcx->module);
    
    // block
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
    ctx->bb = entryBlock;
    
    // create retval if return type not void
    llvm::Value *retval = nullptr;
    llvm::BasicBlock *retBlock = nullptr;
    if (rettype != VoidTy) {
        retval = Build(ctx).CreateAlloca(rettype, 0, "retval");
        ctx->fcx->retAlloca = retval;
    }
    
    retBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", func);
    ctx->fcx->retBlock = retBlock;
    
    // Name params and create argument allocas
    int idx = 0;
    llvm::Function::arg_iterator ai;
    for (ai = func->arg_begin(); ai != func->arg_end(); ++ai, ++idx) {
        std::string name = *paramNames[idx];
        ai->setName(name);
        
        // create alloca
        llvm::AllocaInst *alloca = Build(ctx).CreateAlloca(paramTypes[idx], 0, name + ".addr");
        
        // store initial value
        Build(ctx).CreateStore(ai, alloca);
        
        // add argument to variable symbol table
        (*ctx->scope)[name] = alloca;
    }

    // body
    this->visit(node->block, ctx);
    
    if (rettype == VoidTy) {
        Build(ctx).CreateBr(ctx->fcx->retBlock); // break to return
    }
    
    ctx->bb = retBlock;
    if (rettype != VoidTy) {
        retval = Build(ctx).CreateLoad(retval);
        Build(ctx).CreateRet(retval);
    } else {
        Build(ctx).CreateRetVoid();
    }
    
    ctx->fcx->retBlock = nullptr;
    ctx->fcx->retAlloca = nullptr;
    
    return func;
}

void *RayCodegenVisitor::visitExtern(ExternNode *node, RayCodegenContext *ctx) {
    llvm::Type *rettype = (llvm::Type*)this->visit(node->fntype, ctx);
    llvm::FunctionType *fnType;
    
    std::vector<llvm::Type*> paramTypes;
    std::vector<std::string*> paramNames;
    
    RayCodegenScope *functionScope = new RayCodegenScope(ctx->scope);
    ctx->scope = functionScope;
    
    // Get params
    for (std::vector<Node*>::iterator it = node->params->begin(); it != node->params->end(); ++it) {
        ValueWithType *param = (ValueWithType*)this->visit(*it, ctx);
        paramTypes.push_back(param->type);
        paramNames.push_back((std::string*)param->value);
    }
    
    // Create function
    fnType = llvm::FunctionType::get(rettype, paramTypes, false);
    llvm::Function *func = llvm::Function::Create(fnType, llvm::Function::WeakAnyLinkage, node->name, ctx->fcx->mcx->module);

    
    // Name params and create argument allocas
    int idx = 0;
    llvm::Function::arg_iterator ai;
    for (ai = func->arg_begin(); ai != func->arg_end(); ++ai, ++idx) {
        std::string name = *paramNames[idx];
        ai->setName(name);
        
        // create alloca
        llvm::AllocaInst *alloca = Build(ctx).CreateAlloca(paramTypes[idx], 0, name + ".addr");
        
        // store initial value
        Build(ctx).CreateStore(ai, alloca);
        
        // add argument to variable symbol table
        (*ctx->scope)[name] = alloca;
    }
    
    return func;
}

void *RayCodegenVisitor::visitArray(ArrayNode *node, RayCodegenContext *ctx) {
    llvm::Value *array = nullptr;
    llvm::Type *elemType = nullptr;
    std::vector<llvm::Value*> vals;
    for (std::vector<Node*>::iterator it = node->vals->begin(); it != node->vals->end(); ++it) {
        llvm::Value *val = (llvm::Value*)this->visit(*it, ctx);
        if (!elemType) elemType = val->getType();
    }
    
//    array = llvm::ConstantArray::get(llvm::ArrayType::get(elemType, node->vals->size()), vals);
    return nullptr;
}

void *RayCodegenVisitor::visitHash(HashNode *node, RayCodegenContext *ctx) {
    return nullptr;
}

void *RayCodegenVisitor::visitReturn(ReturnNode *node, RayCodegenContext *ctx) {
    llvm::Value *retAlloca = ctx->fcx->retAlloca; //symbolTable->lookup("retval");
    
    void *retval = this->visit(node->expr, ctx);
    if (node->expr->type == RAY_NODE_ID) {
        if ((*(std::string*)retval).compare("void") != 0) {
            retval = Build(ctx).CreateLoad((*ctx->scope)[*(std::string*)retval]);
        } else {
            // we don't want to load if it's void
            retval = nullptr;
        }
    }
    
    // retval is a nullptr if it's void
    if (retval) {
        Build(ctx).CreateStore((llvm::Value*)retval, retAlloca);
        Build(ctx).CreateBr(ctx->fcx->retBlock);
    }
    
    return nullptr;
}

void *RayCodegenVisitor::visitDecl(DeclNode *node, RayCodegenContext *ctx) {
    std::string *name = new std::string(node->name);
    llvm::Type *type = (llvm::Type*)this->visit(node->vartype, ctx);
    return new ValueWithType { name, type };
}

void *RayCodegenVisitor::visitIf(IfNode *node, RayCodegenContext *ctx) {
    RayCodegenScope *ifScope = new RayCodegenScope(ctx->scope);
    ctx->scope = ifScope;
    
    // Visit condition
    void *cond = this->visit(node->cond, ctx);
    if (node->cond->type == RAY_NODE_ID) {
        cond = Build(ctx).CreateLoad((*ctx->scope)[*(std::string*)cond]);
    }
    
    llvm::Value *condValue = (llvm::Value*)cond;
    if (node->negate) {
        condValue = Build(ctx).CreateNot(condValue);
    }
    
    
    // create blocks
    llvm::Function *func = Build(ctx).GetInsertBlock()->getParent();
    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.then", func, ctx->fcx->retBlock);
    llvm::BasicBlock *elseBlock = nullptr;
    llvm::BasicBlock *endBlock = nullptr;
    
    if (node->elseBlock) {
        elseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.else", func, ctx->fcx->retBlock);
        
        // create conditional break
        Build(ctx).CreateCondBr(condValue, thenBlock, elseBlock);
    } else {
        endBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.end", func, ctx->fcx->retBlock);
        
        // create conditional break
        Build(ctx).CreateCondBr(condValue, thenBlock, endBlock);
    }
    
    // emit then
    ctx->bb = thenBlock;

    // visit block
    this->visit(node->block, ctx);
    if (!ctx->fcx->retAlloca) {
        Build(ctx).CreateBr(ctx->fcx->retBlock);
    }

    // pop off 'if' scope
    ctx->popOffScope();
    
    // visit 'else' block if it exists
    if (node->elseBlock) {
        RayCodegenScope *elseScope = new RayCodegenScope(ctx->scope);
        ctx->scope = elseScope;
        ctx->bb = elseBlock;
        this->visit(node->elseBlock, ctx);
        
        // break to return if it's a void function
        if (!ctx->fcx->retAlloca) {
            Build(ctx).CreateBr(ctx->fcx->retBlock);
        }
        
        ctx->popOffScope();
    } else {
        ctx->bb = endBlock;
    }

    
    return endBlock;
}

void *RayCodegenVisitor::visitSequence(SequenceNode *node, RayCodegenContext *ctx) {
    void *retVal = nullptr;
    
    for (std::vector<Node*>::iterator it = node->nodes->begin(); it != node->nodes->end(); ++it) {
        retVal = this->visit(*it, ctx);
    }
    return retVal;
}