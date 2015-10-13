////
////  JSCodegen.h
////  rayc
////
////  Created by Jeremy Jacobson on 11/15/14.
////  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
////
//
//#ifndef __rayc__JSCodegen__
//#define __rayc__JSCodegen__
//
//#include "Codegen.h"
//
//typedef enum {
//    JS_TYPE_NUMBER,
//    JS_TYPE_BOOL,
//    JS_TYPE_STRING,
//    JS_TYPE_OBJECT,
//    JS_TYPE_FUNCTION,
//    JS_TYPE_ID
//} JSType;
//
//class JSValue {
//
//public:
//    JSType type;
//    
//    JSValue(JSType type);
//};
//
//class JSInt : public JSValue {
//    
//public:
//    long val;
//    
//    JSInt(long val) : JSValue(JS_TYPE_NUMBER) { this->val = val; }
//    std::string to_str() { return std::to_string(this->val); }
//};
//
//class JSFloat : public JSValue {
//    
//public:
//    double val;
//    
//    JSFloat(double val) : JSValue(JS_TYPE_NUMBER) { this->val = val; }
//    std::string to_str() { return std::to_string(this->val); }
//};
//
//class JSBool : public JSValue {
//    
//public:
//    bool val;
//    
//    JSBool(bool val) : JSValue(JS_TYPE_BOOL) { this->val = val; }
//    std::string to_str() { return (this->val) ? "true" : "false"; }
//};
//
//class JSString : public JSValue {
//    
//public:
//    std::string *val;
//    
//    JSString(std::string *val) : JSValue(JS_TYPE_STRING) { this->val = val; }
//    std::string to_str() { return *this->val; }
//};
//
//class JSIDValue : public JSValue {
//    std::string *val;
//
//public:
//    JSIDValue(std::string *val) : JSValue(JS_TYPE_ID) { this->val = val; }
//    std::string to_str() { return *this->val; }
//};
//
//class JSObject : public JSValue {
//    
//public:
//    std::map<std::string, JSValue*> *val;
//    
//    JSObject(std::map<std::string, JSValue*> *val) : JSValue(JS_TYPE_OBJECT) { this->val = val; }
//    std::string to_str() {
//        std::string s("{");
//        
//        for (std::map<std::string, JSValue*>::iterator it = this->val->begin(); it != this->val->end(); ++it) {
////            s += it.;
//        }
//        
//        s += "}";
//        return s;
//    }
//};
//
//class JSFunction : public JSValue {
//    
//public:
//    std::vector<std::string> *args;
////    std::map<std::string, JSValue*> *val;
//    
//    JSFunction(std::vector<std::string> *args) : JSValue(JS_TYPE_FUNCTION) { this->args = args; }
//};
//
//class JSBlock {
//
//public:
//    std::vector<JSValue*> *vals;
//    
//    JSBlock() { this->vals = new std::vector<JSValue*>(); }
//};
//
//class JSBuilder {
//    JSBlock *insertPoint;
//    
//public:
//    
//    JSBuilder() {}
//    void setInsertPoint(JSBlock *insertPoint) { this->insertPoint = insertPoint; }
////    JSValue *createOp(JSValue *lhs, JSValue *rhs, std::string op) {
////        this->insertPoint
////    }
//    
//};
//
//
//
//class RayJSModuleContext {
//    
//public:
////    llvm::Module *module;
//    
//    RayJSModuleContext();
////    llvm::IRBuilder<> builder();
//};
//
//class RayJSFunctionContext {
//    
//public:
//    RayJSModuleContext *mcx;
//    JSValue *retAlloca;
////    llvm::BasicBlock *retBlock;
//    
//    RayJSFunctionContext(RayJSModuleContext *mcx);
//};
//
//class RayJSCodegenScope : public std::map<std::string, JSValue*> {
//    
//public:
//    RayJSCodegenScope *parentScope;
//    
//    RayJSCodegenScope();
//    RayJSCodegenScope(RayJSCodegenScope *parentScope);
//};
//
//class RayJSCodegenContext {
//    
//public:
//    JSBlock *block;
//    RayJSCodegenScope *scope;
//    RayJSFunctionContext *fcx;
//    
//    //    RayCodegenContext(RayCodegenContext *ctx);
//    RayJSCodegenContext(JSBlock block, RayJSCodegenScope *scope, RayJSFunctionContext *fcx);
//    JSValue *getValue(std::string name);
//    RayJSCodegenScope *popOffScope();
//};
//
//class RayJSCodegenVisitor : public RayCodegenVisitor {
//    
//public:
//    RayJSCodegenVisitor() : RayCodegenVisitor() {}
//    void *visit(Node *node, RayJSCodegenContext *ctx);
//    void *visitBlock(BlockNode *node, RayJSCodegenContext *ctx);
//    void *visitID(IDNode *node, RayJSCodegenContext *ctx);
//    void *visitInt(IntNode *node, RayJSCodegenContext *ctx);
//    void *visitFloat(FloatNode *node, RayJSCodegenContext *ctx);
//    void *visitString(StringNode *node, RayJSCodegenContext *ctx);
//    void *visitChar(CharNode *node, RayJSCodegenContext *ctx);
//    void *visitBool(BoolNode *node, RayJSCodegenContext *ctx);
//    void *visitType(TypeNode *node, RayJSCodegenContext *ctx);
//    void *visitSlot(SlotNode *node, RayJSCodegenContext *ctx);
//    void *visitCall(CallNode *node, RayJSCodegenContext *ctx);
//    void *visitWhile(WhileNode *node, RayJSCodegenContext *ctx);
//    void *visitUnaryOp(UnaryOpNode *node, RayJSCodegenContext *ctx);
//    void *visitBinaryOp(BinaryOpNode *node, RayJSCodegenContext *ctx);
//    void *visitFunction(FunctionNode *node, RayJSCodegenContext *ctx);
//    void *visitArray(ArrayNode *node, RayJSCodegenContext *ctx);
//    void *visitHash(HashNode *node, RayJSCodegenContext *ctx);
//    void *visitReturn(ReturnNode *node, RayJSCodegenContext *ctx);
//    void *visitDecl(DeclNode *node, RayJSCodegenContext *ctx);
//    void *visitIf(IfNode *node, RayJSCodegenContext *ctx);
//};
//
//
//#endif /* defined(__rayc__JSCodegen__) */
