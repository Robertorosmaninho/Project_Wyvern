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

#include "LazyProfitability.h"

void LazyProfitability::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool LazyProfitability::runOnModule(Module &M){
  //The code goes here
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
			"Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
			false, false);
