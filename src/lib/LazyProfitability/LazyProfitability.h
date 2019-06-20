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
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"

using namespace llvm;

struct LazyProfitability : public ModulePass{
private:
  int _id_function = 0, _n_functions = 0, _opportunity = 0;
  std::string _caller_function, _called_function, _argument_function;
  bool _lazy_arguments, _caller_lazy_arguments;

  std::map<int,std::string> _caller_functions_map; //Id, Function Name
  std::map<int, std::string> _called_functions_map; //Id, Function Name
  std::map<int, bool> _has_lazy_arguments; //Id, If has lazy arguments

  //The main ideia is to print a csv file with the follow columns:
  //Id, CallerFunctionName, CalledFunctionName, HasLazyArguments
  //
  //And other csv file that resumes the content of the program
  //NumberOfFunctions, CallerOfFunctionsWith, NumberOfCalledFunctions, 
  //                                         NumberOfFunctionsWithLazyArguments
	
public:
static char ID;
LazyProfitability() : ModulePass(ID){}

void getAnalysisUsage(AnalysisUsage &AU) const;
virtual bool runOnModule(Module &M);
};

#endif
