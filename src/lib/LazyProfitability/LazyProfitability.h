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
#include "llvm/Pass.h"

#include <fstream>
#include <cstring>
#include <sstream>

using namespace llvm;

struct LazyProfitability : public FunctionPass{
private:
  int *_id_function = 0, _n_functions = 0, _n_call = 0, *_value_opportunity = 0, 
      _function_opportunity = 0, _call_function = 0, *_function_value_used = 0;
  std::string _analyzed_function, _caller_function, _called_function, 
                                                            _argument_function;
  bool _lazy_arguments, _caller_lazy_arguments;
  
  std::map<int*, std::string> _function_analyzed_map; //Id, Function Name
  std::map<int*, std::string> _function_caller_map; //Id, Function Name
  std::map<int*, std::string> _function_called_map; //Id, Function Name
  std::map<int*, int> _has_function_as_arguments; //Id, If has lazy arguments
  std::map<int*, int> _has_function_value_as_arguments; //Id, If an value can
                                                        //be lazy
  std::map<std::string, int> _has_not_arguments_post_dom;
  //NameOfCallerFunction, HasArgumentsThatDoesn'tPostDomEntry

  public:
  void case_1_Value_as_Argument_Load(Use *argOP, int *_function_value_used,
                             std::map<Value*, Value*> *_stored_value_function); 

  void case_1_Value_as_Argument_Store();
  void case_2_Function_as_Agument();

static char ID;
LazyProfitability() : FunctionPass(ID){}

void getAnalysisUsage(AnalysisUsage &AU) const;
virtual bool runOnFunction(Function &F);
};

#endif
