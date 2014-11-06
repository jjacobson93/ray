//
//  main.cpp
//  rayc
//
//  Created by Jeremy Jacobson on 10/31/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "Codegen.h"

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
                codegen->gen(root);
            }
//            while (lex->scan()) {
//                std::cout << lex->lineno << ": ";
//                std::cout << lex->tok->inspect() << std::endl;
//            }
        } else {
            std::cout << "Error opening file" << std::endl;
        }
    } else {
        std::cout << "Usage: rayc <infile>" << std::endl;
    }
    return 0;
}
