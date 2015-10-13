//
//  LLCWrapper.h
//  rayc
//
//  Created by Jeremy Jacobson on 11/9/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef __rayc__LLCWrapper__
#define __rayc__LLCWrapper__

#include <llvm/IR/Module.h>

int LLCMain(int argc, char **argv, llvm::Module *mod);

#endif /* defined(__rayc__LLCWrapper__) */
