//
//  main.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include "Lexer.h"
#include "Parser.h"
#include "Codegen.h"
#include "Backend.h"

int main(int argc, const char * argv[]) {
    if (argc > 1) {
        std::ifstream infile(argv[1]);
        if (infile.is_open()) {
            std::string source((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
            std::string filename(argv[1]);
            
            RayLexer *lex = new RayLexer(source, filename);
            RayParser *parser = new RayParser(lex);
            BlockNode *root = parser->parse();
            if (!root) {
                parser->reportError();
            } else {
                RayCodegen *codegen = new RayCodegen();
                void *result = codegen->gen(root, RAY_TARGET_NATIVE);
                llvm::Module *module = (llvm::Module*)result;
                std::string modstr;
                llvm::raw_string_ostream os(modstr);
                module->print(os, nullptr);
                std::ofstream output;
                output.open((argc > 2) ? argv[2] : "out.ll");
                output << os.str();
                output.close();
//                RayTarget target = (argc > 2 && !strcmp(argv[2], "js")) ? RAY_TARGET_JS : RAY_TARGET_NATIVE;
//                void *result = codegen->gen(root, target);
//                
//                if (target == RAY_TARGET_NATIVE) {
//                    llvm::Module *module = (llvm::Module*)result;
//                    std::string modstr;
//                    llvm::raw_string_ostream os(modstr);
//                    module->print(os, nullptr);
//                    std::ofstream output;
//                    output.open((argc == 3)argv[2]);
//                    output << os.str();
//                    output.close();
//                    
////                    FILE *proc = popen("/usr/local/bin/llc out.ll -o -", "r");
////                    char buffer[255];
////                    std::string asmOut;
////                    while ( fgets(buffer, 255, proc) != NULL )
////                        asmOut.append(buffer);
////                    pclose(proc);
////                    
////                    output.open("asmout.s");
////                    output << asmOut;
////                    output.close();
////                    
////                    popen("clang asmout.s", "r");
//                } else {
//                    std::ofstream output;
//                    output.open("out.js");
//                    output << (std::string*)result;
//                    output.close();
//                }
                
//                module->dump();
//                LLCMain(argc, (char**)argv, module);
            }
//            while (lex->scan()) {
//                std::cout << lex->lineno << ": ";
//                std::cout << lex->tok->inspect() << std::endl;
//            }
        } else {
            std::cout << "Error opening file" << std::endl;
        }
    } else {
        std::cout << "Usage: rayc <infile> [<target>]" << std::endl;
    }
    return 0;
}
