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

#include <llvm/Analysis/PostDominators.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"

#include "lib.h"
#include "../PDG/PDGAnalysis.h"
#include "../DFS/DFS.h"

#include <fstream>
#include <cstring>
#include <sstream>

using namespace llvm;

struct LazyProfitability : public ModulePass{
private:
  int _id_function = 0, _n_functions = 0, _n_call = 0, _value_opportunity = 0, 
      _function_opportunity = 0, _call_function = 0, _function_value_used = 0;
  std::string _analyzed_function, _caller_function, _called_function, 
                                                            _argument_function;
  bool _lazy_arguments, _caller_lazy_arguments;
  
  //Id, Function, num args, num args that can be lazy
  std::map<std::pair <int, Function*>, std::pair<int, int>> _function_map;
  
  std::set<Instruction*> _dependences;
  std::vector<std::set<Instruction*>> _dependeces_list;

  public:

static char ID;
LazyProfitability() : ModulePass(ID){}

void getAnalysisUsage(AnalysisUsage &AU) const;
virtual bool runOnModule(Module &M);
};

#endif
