//=_ LazyProfitability.h - Profitability of Use Lazy Evaluation on Program _=//
//
//          Compilers Laboratoy at Federal University of Minas Gerais
//
//===____________________________________________________________________===//
///
///
/// This file contains the declaration of LazyProfitability class which counts
/// the opportunities to use Lazy Evaluation on a program.
//===____________________________________________________________________===//

#ifndef LLVM_LIB_TRANSFORMS_LAZYPROFITABILITY_H
#define LLVM_LIB_TRANSFORMS_LAZYPROFITABILITY_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"

using namespace llvm;

struct LazyProfitability : public ModulePass{
private:
	int _opportunity;

public:
static char ID;
LazyProfitability() : ModulePass(ID){}

void getAnalysisUsage(AnalysisUsage &AU) const;
virtual bool runOnModule(Module &M);
};

#endif
