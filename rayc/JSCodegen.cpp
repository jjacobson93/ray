////
////  JSCodegen.cpp
////  rayc
////
////  Created by Jeremy Jacobson on 11/15/14.
////  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
////
//
//#include "JSCodegen.h"
//#include <iostream>
//#include <sstream>
//
//#define VISIT(type) \
//return this->visit##type((type##Node *) node, ctx);
//
//void *error(std::string str) {
//    std::cerr << str << std::endl;
//    exit(1);
//    return NULL;
//}
//
////JSValue *Build(RayJSCodegenContext *ctx, JSValue *val) {
////    llvm::IRBuilder<> builder = ctx->fcx->mcx->builder();
////    ctx->block->vals->
////    return val;
////}
//
//RayJSCodegenContext::RayJSCodegenContext(RayJSCodegenScope *scope, RayJSFunctionContext *fcx){
//    this->scope = new RayJSCodegenScope(scope);
//    this->fcx = fcx;
//}
//
//JSValue *RayJSCodegenContext::getValue(std::string name) {
//    return (*this->scope)[name];
////    return this->fcx->mcx->module->getFunction(fname);
//}
//
//RayJSCodegenScope *RayJSCodegenContext::popOffScope() {
//    RayJSCodegenScope *parentScope = this->scope->parentScope;
//    delete this->scope;
//    this->scope = parentScope;
//    return parentScope;
//}
//
//RayJSFunctionContext::RayJSFunctionContext(RayJSModuleContext *mcx) {
//    this->mcx = mcx;
//}
//
////RayJSModuleContext::RayJSModuleContext(llvm::Module *module) {
////    this->module = module;
////}
//
////llvm::IRBuilder<> RayJSModuleContext::builder() {
////    return llvm::IRBuilder<>(this->module->getContext());
////}
//
//RayJSCodegenScope::RayJSCodegenScope() : std::map<std::string, JSValue *>() {
//    this->parentScope = nullptr;
//}
//
//RayJSCodegenScope::RayJSCodegenScope(JSBlock block, RayJSCodegenScope *parentScope) : std::map<std::string, JSValue *>(*parentScope) {
//    this->block = block;
//    this->parentScope = parentScope;
//}
//
//void *RayJSCodegenVisitor::visit(Node *node, RayJSCodegenContext *ctx) {
//    switch (node->type) {
//        case RAY_NODE_BLOCK: VISIT(Block);
//        case RAY_NODE_ID: VISIT(ID);
//        case RAY_NODE_DECL: VISIT(Decl);
//        case RAY_NODE_INT: VISIT(Int);
//        case RAY_NODE_FLOAT: VISIT(Float);
//        case RAY_NODE_STRING: VISIT(String);
//        case RAY_NODE_CHAR: VISIT(Char);
//        case RAY_NODE_BOOL: VISIT(Bool);
//        case RAY_NODE_TYPE: VISIT(Type);
//        case RAY_NODE_SLOT: VISIT(Slot);
//        case RAY_NODE_CALL: VISIT(Call);
//        case RAY_NODE_IF: VISIT(If);
//        case RAY_NODE_WHILE: VISIT(While);
//        case RAY_NODE_UNARY_OP: VISIT(UnaryOp);
//        case RAY_NODE_BINARY_OP: VISIT(BinaryOp);
//        case RAY_NODE_FUNCTION: VISIT(Function);
//        case RAY_NODE_ARRAY: VISIT(Array);
//        case RAY_NODE_HASH: VISIT(Hash);
//        case RAY_NODE_RETURN: VISIT(Return);
////        case RAY_NODE_SEQUENCE: VISIT(Sequence);
//        default: return NULL;
//    }
//}
//
//void *RayJSCodegenVisitor::visitBlock(BlockNode *node, RayJSCodegenContext *ctx) {
//    void *retVal = nullptr;
//    
//    RayJSCodegenScope *blockScope = new RayJSCodegenScope(ctx->scope);
//    ctx->scope = blockScope;
//    for (std::vector<Node*>::iterator it = node->stmts->begin(); it != node->stmts->end(); ++it) {
//        retVal = this->visit(*it, ctx);
//    }
//    ctx->popOffScope();
//    return retVal;
//}
//
//void *RayJSCodegenVisitor::visitID(IDNode *node, RayJSCodegenContext *ctx) {
//    //    if (std::find(keywords, keywords + 16, node->val) != keywords + 16) {
//    //        return new std::string(node->val);
//    //    }
//    //
//    //    // Look up variable
//    //    llvm::Value *val = namedValues[node->val];
//    //    if (!val) return new std::string(node->val);
//    //
//    //    // Load value
//    //    return builder.CreateLoad(val, node->val.c_str());
//    return node->val;
//}
//
//void *RayJSCodegenVisitor::visitInt(IntNode *node, RayJSCodegenContext *ctx) {
//    return new JSInt(node->val);
//}
//
//void *RayJSCodegenVisitor::visitFloat(FloatNode *node, RayJSCodegenContext *ctx) {
//    return new JSFloat(node->val);
//}
//
//void *RayJSCodegenVisitor::visitString(StringNode *node, RayJSCodegenContext *ctx) {
//    return new JSString(node->val);
//}
//
//void *RayJSCodegenVisitor::visitChar(CharNode *node, RayJSCodegenContext *ctx) {
//    std::string *s = new std::string();
//    s += node->val;
//    return new JSString(s);
//}
//
//void *RayJSCodegenVisitor::visitBool(BoolNode *node, RayJSCodegenContext *ctx) {
//    return new JSBool(node->val);
//}
//
//void *RayJSCodegenVisitor::visitType(TypeNode *node, RayJSCodegenContext *ctx) {
//    for (std::vector<Node*>::iterator it = node->types->begin(); it != node->types->end(); ++it) {
//        std::string *tyname = (std::string*)this->visit(*it, ctx);
//        return tyname;
////        if (tyname->compare("int") == 0) {
////            return new std::string("Number";
////        } else if (tyname->compare("float") == 0) {
////            return ";
////        } else if (tyname->compare("bool") == 0) {
////            return i1;
////        } else if (tyname->compare("string") == 0) {
////            return llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext());
////        } else {
////            return llvm::Type::getVoidTy(llvm::getGlobalContext());
////        }
//    }
////
////    return llvm::Type::getVoidTy(llvm::getGlobalContext());
//    return nullptr;
////    return tyname;
//}
//
//void *RayJSCodegenVisitor::visitSlot(SlotNode *node, RayJSCodegenContext *ctx) {
//    return nullptr;
//}
//
//void *RayJSCodegenVisitor::visitCall(CallNode *node, RayJSCodegenContext *ctx) {
//    std::string *res = new std::string();
//    std::string *callee = (std::string*)this->visit(node->expr, ctx);
//    JSFunction *func = (JSFunction*)ctx->getValue(*callee);
//    if (!func) return error("unknown function: " + *callee);
//    
//    *res += *callee + "(";
//    
////    std::vector<llvm::Value*> args;
//    if (node->args->vec->size()) {
//        Node *curr;
//        for (std::vector<Node*>::iterator it = node->args->vec->begin(); it != node->args->vec->end(); ++it) {
//            curr = *it;
//            JSValue *arg = (JSValue*)this->visit(*it, ctx);
//            if (curr->type == RAY_NODE_ID) {
//                std::string argAsString = *(std::string*)arg;
//                if (ctx->scope->find(argAsString) == ctx->scope->end()) return error("Unknown variable: " + argAsString);
////                arg = new JSIDValue(new std::string(argAsString));
////                arg = Build(ctx).CreateLoad((*ctx->scope)[argAsString]);
//            }
//            *res += arg->to_str();
//            
////            args.push_back((JSValue*)arg);
//            
//        }
//    }
//    
//    *res += ");";
//    
//    return res;
//    
////    return Build(ctx).CreateCall(func, args);
//}
//
//void *RayJSCodegenVisitor::visitWhile(WhileNode *node, RayJSCodegenContext *ctx) {
//    bool negate = node->negate;
//    
//    RayJSCodegenScope *whileScope = new RayJSCodegenScope(ctx->scope);
//    ctx->scope = whileScope;
//    
////    llvm::Function *func = Build(ctx).GetInsertBlock()->getParent();
////    // creat blocks
////    llvm::BasicBlock *condBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while.cond", func, ctx->fcx->retBlock);
////    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while.body", func, ctx->fcx->retBlock);
////    llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while.end", func, ctx->fcx->retBlock);
////    
////    Build(ctx).CreateBr(condBlock);
////    
////    ctx->bb = condBlock;
//    
//    void *cond = this->visit(node->cond, ctx);
////    if (node->cond->type == RAY_NODE_ID) {
////        cond = Build(ctx).CreateLoad((*ctx->scope)[*(std::string*)cond]);
////    }
//    
//    
//    
//    llvm::Value *condValue = (llvm::Value*)cond;
//    
//    if (negate) {
//        condValue = Build(ctx).CreateNot(condValue);
//    }
//    
//    // create conditional break
//    Build(ctx).CreateCondBr(condValue, bodyBlock, endBlock);
//    
//    // emit then
//    ctx->bb = bodyBlock;
//    
//    // visit block
//    this->visit(node->block, ctx);
//    Build(ctx).CreateBr(condBlock);
//    
//    // pop off 'while' scope
//    ctx->popOffScope();
//    
//    // set end block as context's bb
//    ctx->bb = endBlock;
//    
//    return endBlock;
//}
//
//void *RayJSCodegenVisitor::visitUnaryOp(UnaryOpNode *node, RayJSCodegenContext *ctx) {
//    void *expr = this->visit(node->expr, ctx);
//    
//    llvm::Value *alloca = nullptr;
//    if (node->expr->type == RAY_NODE_ID) {
//        std::string *name = (std::string*)expr;
//        alloca = (*ctx->scope)[*name];
//        if (!alloca) return error("variable '" + *name + "' is undefined");
//        
//        // load
//        expr = Build(ctx).CreateLoad(alloca);
//    }
//    
//    llvm::Value *exprValue = (llvm::Value*)expr;
//    llvm::Type *exprType = exprValue->getType();
//    
//    llvm::Value *rhs;
//    
//    switch (node->op) {
//        case RAY_TOKEN_OP_INCR:
//            if (exprType == i32) {
//                rhs = Build(ctx).CreateAdd(exprValue, llvm::ConstantInt::get(i32, 1));
//            } else {
//                rhs = Build(ctx).CreateFAdd(exprValue, llvm::ConstantFP::get(f64, 1.0));
//            }
//            Build(ctx).CreateStore(rhs, alloca);
//            return (node->postfix) ? exprValue : rhs;
//        case RAY_TOKEN_OP_DECR:
//            if (exprType == i32) {
//                rhs = Build(ctx).CreateSub(exprValue, llvm::ConstantInt::get(i32, 1));
//            } else {
//                rhs = Build(ctx).CreateFSub(exprValue, llvm::ConstantFP::get(f64, 1.0));
//            }
//            Build(ctx).CreateStore(rhs, alloca);
//            return (node->postfix) ? exprValue : rhs;
//        case RAY_TOKEN_OP_BIT_NOT:
//            if (exprType == i32) {
//                rhs = Build(ctx).CreateAdd(exprValue, llvm::ConstantInt::get(i32, 1));
//            } else {
//                return error("cannot bitwise not type");
//            }
//            Build(ctx).CreateStore(rhs, alloca);
//            return (node->postfix) ? exprValue : rhs;
//        case RAY_TOKEN_OP_PLUS:
//            return exprValue;
//        case RAY_TOKEN_OP_MINUS:
//            if (exprType == i32) {
//                rhs = Build(ctx).CreateNeg(exprValue);
//            } else {
//                rhs = Build(ctx).CreateFNeg(exprValue);
//            }
//            Build(ctx).CreateStore(rhs, alloca);
//            return (node->postfix) ? exprValue : rhs;
//        case RAY_TOKEN_OP_NOT:
//            if (exprType == i32) {
//                rhs = Build(ctx).CreateICmpNE(exprValue, llvm::ConstantInt::get(i32, 0));
//            } else {
//                rhs = Build(ctx).CreateFCmpONE(exprValue, llvm::ConstantFP::get(f64, 0.0));
//            }
//            Build(ctx).CreateStore(rhs, alloca);
//            return (node->postfix) ? exprValue : rhs;
//        default:
//            return error("invalid operator");
//    }
//    
//    return expr;
//}
//
//bool opIsAssignment(RayJSTokenType op) {
//    return op == RAY_TOKEN_OP_ASSIGN ||
//    op == RAY_TOKEN_OP_PLUS_ASSIGN ||
//    op == RAY_TOKEN_OP_MINUS_ASSIGN ||
//    op == RAY_TOKEN_OP_MUL_ASSIGN ||
//    op == RAY_TOKEN_OP_DIV_ASSIGN ||
//    op == RAY_TOKEN_OP_AND_ASSIGN ||
//    op == RAY_TOKEN_OP_OR_ASSIGN;
//}
//
//void *RayJSCodegenVisitor::visitBinaryOp(BinaryOpNode *node, RayJSCodegenContext *ctx) {
//    void *lhs = this->visit(node->left, ctx);
//    void *rhs = this->visit(node->right, ctx);
//    
//    // if lhs is an ID, we need to load it or resolve it
//    if (node->left->type == RAY_NODE_ID) {
//        if (!node->var) { // not an assign; we only want to look up previously defined variables
//            std::string *lname = (std::string*)lhs;
//            llvm::Value *lalloca = (*ctx->scope)[*lname];
//            if (!lalloca) return error("variable '" + *lname + "' is undefined");
//            
//            if (!opIsAssignment(node->op)) {
//                // load
//                lhs = Build(ctx).CreateLoad(lalloca);
//            } else {
//                lhs = lalloca;
//            }
//        }
//    }
//    
//    // if rhs is an ID we need to load it or resolve it
//    if (node->right->type == RAY_NODE_ID) {
//        if (!node->var) { // not an assign; we only want to look up previously defined variables
//            std::string *rname = (std::string*)rhs;
//            llvm::Value *ralloca = (*ctx->scope)[*rname];
//            if (!ralloca) return error("variable '" + *rname + "' is undefined");
//            
//            if (!opIsAssignment(node->op)) {
//                // load
//                rhs = Build(ctx).CreateLoad(ralloca);
//            } else {
//                rhs = ralloca;
//            }
//        }
//    }
//    
//    llvm::Value *lhsValue = (llvm::Value*)lhs;
//    llvm::Value *rhsValue = (llvm::Value*)rhs;
//    
//    llvm::Type *lhsType = lhsValue->getType();
//    llvm::Type *rhsType = rhsValue->getType();
//    
//    if (lhsType && rhsType && !lhsType->isPointerTy() && !rhsType->isPointerTy() && lhsType != rhsType) {
//        return error("ray currently does not support implicit casting");
//    }
//    
//    switch (node->op) {
//        case RAY_TOKEN_OP_PLUS:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateAdd(lhsValue, rhsValue);
//            return Build(ctx).CreateFAdd(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_MINUS:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateSub(lhsValue, rhsValue);
//            return Build(ctx).CreateFSub(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_MUL:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateMul(lhsValue, rhsValue);
//            return Build(ctx).CreateFMul(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_DIV:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateSDiv(lhsValue, rhsValue);
//            return Build(ctx).CreateFDiv(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_MOD:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateSRem(lhsValue, rhsValue);
//            return Build(ctx).CreateFRem(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_POW:
//            return Build(ctx).CreateCall((lhsType->isIntegerTy() ? ctx->getFunction("__RAY_RUNTIME_POW_INT") : ctx->getFunction("__RAY_RUNTIME_POW_FLOAT")), { lhsValue, rhsValue });
//        case RAY_TOKEN_OP_AND:
//            return Build(ctx).CreateAnd(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_OR:
//            return Build(ctx).CreateOr(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_EQ:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateICmpEQ(lhsValue, rhsValue);
//            return Build(ctx).CreateFCmpOEQ(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_NEQ:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateICmpNE(lhsValue, rhsValue);
//            return Build(ctx).CreateFCmpONE(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_GT:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateICmpSGT(lhsValue, rhsValue);
//            return Build(ctx).CreateFCmpOGT(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_LT:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateICmpSLT(lhsValue, rhsValue);
//            return Build(ctx).CreateFCmpOLT(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_GTE:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateICmpSGE(lhsValue, rhsValue);
//            return Build(ctx).CreateFCmpOGE(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_LTE:
//            if (lhsType->isIntegerTy())
//                return Build(ctx).CreateICmpSLE(lhsValue, rhsValue);
//            return Build(ctx).CreateFCmpOLE(lhsValue, rhsValue);
//        case RAY_TOKEN_OP_ASSIGN: {
//            if (node->var) {
//                llvm::Value *alloca = Build(ctx).CreateAlloca(rhsType, 0, *(std::string*)lhs);
//                Build(ctx).CreateStore(rhsValue, alloca);
//                (*ctx->scope)[*(std::string*)lhs] = alloca;
//            } else {
//                Build(ctx).CreateStore(rhsValue, lhsValue);
//            }
//        }
//        default:
//            break;
//    }
//    
//    return nullptr;
//}
//
//void *RayJSCodegenVisitor::visitFunction(FunctionNode *node, RayJSCodegenContext *ctx) {
//    llvm::Type *rettype = (llvm::Type*)this->visit(node->fntype, ctx);
//    llvm::FunctionType *fnType;
//    
//    std::vector<llvm::Type*> paramTypes;
//    std::vector<std::string*> paramNames;
//    
//    RayJSCodegenScope *functionScope = new RayJSCodegenScope(ctx->scope);
//    ctx->scope = functionScope;
//    
//    // Get params
//    for (std::vector<Node*>::iterator it = node->params->begin(); it != node->params->end(); ++it) {
//        ValueWithType *param = (ValueWithType*)this->visit(*it, ctx);
//        paramTypes.push_back(param->type);
//        paramNames.push_back((std::string*)param->value);
//    }
//    
//    // Create function
//    fnType = llvm::FunctionType::get(rettype, paramTypes, false);
//    llvm::Function *func = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, node->name, ctx->fcx->mcx->module);
//    
//    // block
//    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
//    ctx->bb = entryBlock;
//    
//    // create retval if return type not void
//    llvm::Value *retval = nullptr;
//    llvm::BasicBlock *retBlock = nullptr;
//    if (rettype != VoidTy) {
//        retval = Build(ctx).CreateAlloca(rettype, 0, "retval");
//        ctx->fcx->retAlloca = retval;
//    }
//    
//    retBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", func);
//    ctx->fcx->retBlock = retBlock;
//    
//    // Name params and create argument allocas
//    int idx = 0;
//    llvm::Function::arg_iterator ai;
//    for (ai = func->arg_begin(); ai != func->arg_end(); ++ai, ++idx) {
//        std::string name = *paramNames[idx];
//        ai->setName(name);
//        
//        // create alloca
//        llvm::AllocaInst *alloca = Build(ctx).CreateAlloca(paramTypes[idx], 0, name + ".addr");
//        
//        // store initial value
//        Build(ctx).CreateStore(ai, alloca);
//        
//        // add argument to variable symbol table
//        (*ctx->scope)[name] = alloca;
//    }
//    
//    // body
//    this->visit(node->block, ctx);
//    
//    ctx->bb = retBlock;
//    if (rettype != VoidTy) {
//        retval = Build(ctx).CreateLoad(retval);
//        Build(ctx).CreateRet(retval);
//    } else {
//        Build(ctx).CreateRetVoid();
//    }
//    
//    ctx->fcx->retBlock = nullptr;
//    ctx->fcx->retAlloca = nullptr;
//    
//    return func;
//}
//
//void *RayJSCodegenVisitor::visitArray(ArrayNode *node, RayJSCodegenContext *ctx) {
//    llvm::Value *array = nullptr;
//    llvm::Type *elemType = nullptr;
//    std::vector<llvm::Value*> vals;
//    for (std::vector<Node*>::iterator it = node->vals->begin(); it != node->vals->end(); ++it) {
//        llvm::Value *val = (llvm::Value*)this->visit(*it, ctx);
//        if (!elemType) elemType = val->getType();
//    }
//    
//    //    array = llvm::ConstantArray::get(llvm::ArrayType::get(elemType, node->vals->size()), vals);
//    return nullptr;
//}
//
//void *RayJSCodegenVisitor::visitHash(HashNode *node, RayJSCodegenContext *ctx) {
//    return nullptr;
//}
//
//void *RayJSCodegenVisitor::visitReturn(ReturnNode *node, RayJSCodegenContext *ctx) {
//    llvm::Value *retAlloca = ctx->fcx->retAlloca; //symbolTable->lookup("retval");
//    
//    void *retval = this->visit(node->expr, ctx);
//    if (node->expr->type == RAY_NODE_ID) {
//        if ((*(std::string*)retval).compare("void") != 0) {
//            retval = Build(ctx).CreateLoad((*ctx->scope)[*(std::string*)retval]);
//        } else {
//            // we don't want to load if it's void
//            retval = nullptr;
//        }
//    }
//    
//    // retval is a nullptr if it's void
//    if (retval) {
//        Build(ctx).CreateStore((llvm::Value*)retval, retAlloca);
//        Build(ctx).CreateBr(ctx->fcx->retBlock);
//    }
//    
//    return nullptr;
//}
//
//void *RayJSCodegenVisitor::visitDecl(DeclNode *node, RayJSCodegenContext *ctx) {
//    std::string *name = new std::string(node->name);
//    llvm::Type *type = (llvm::Type*)this->visit(node->vartype, ctx);
//    return new ValueWithType { name, type };
//}
//
//void *RayJSCodegenVisitor::visitIf(IfNode *node, RayJSCodegenContext *ctx) {
//    RayJSCodegenScope *ifScope = new RayJSCodegenScope(ctx->scope);
//    ctx->scope = ifScope;
//    
//    // Visit condition
//    void *cond = this->visit(node->cond, ctx);
//    if (node->cond->type == RAY_NODE_ID) {
//        cond = Build(ctx).CreateLoad((*ctx->scope)[*(std::string*)cond]);
//    }
//    
//    llvm::Value *condValue = (llvm::Value*)cond;
//    if (node->negate) {
//        condValue = Build(ctx).CreateNot(condValue);
//    }
//    
//    
//    // create blocks
//    llvm::Function *func = Build(ctx).GetInsertBlock()->getParent();
//    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.then", func, ctx->fcx->retBlock);
//    llvm::BasicBlock *elseBlock = nullptr;
//    llvm::BasicBlock *endBlock = nullptr;
//    
//    if (node->elseBlock) {
//        elseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.else", func, ctx->fcx->retBlock);
//        
//        // create conditional break
//        Build(ctx).CreateCondBr(condValue, thenBlock, elseBlock);
//    } else {
//        endBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "if.end", func, ctx->fcx->retBlock);
//        
//        // create conditional break
//        Build(ctx).CreateCondBr(condValue, thenBlock, endBlock);
//    }
//    
//    // emit then
//    ctx->bb = thenBlock;
//    
//    // visit block
//    this->visit(node->block, ctx);
//    if (!ctx->fcx->retAlloca) {
//        Build(ctx).CreateBr(ctx->fcx->retBlock);
//    }
//    
//    // pop off 'if' scope
//    ctx->popOffScope();
//    
//    // visit 'else' block if it exists
//    if (node->elseBlock) {
//        RayJSCodegenScope *elseScope = new RayJSCodegenScope(ctx->scope);
//        ctx->scope = elseScope;
//        ctx->bb = elseBlock;
//        this->visit(node->elseBlock, ctx);
//        
//        // break to return if it's a void function
//        if (!ctx->fcx->retAlloca) {
//            Build(ctx).CreateBr(ctx->fcx->retBlock);
//        }
//        
//        ctx->popOffScope();
//    } else {
//        ctx->bb = endBlock;
//    }
//    
//    
//    return endBlock;
//}