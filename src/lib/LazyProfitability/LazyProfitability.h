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
  int _id_function = 0, _n_functions = 0, _n_call = 0, _value_opportunity = 0, 
      _function_opportunity = 0, _call_function = 0, _function_value_used = 0;
  std::string _analyzed_function, _caller_function, _called_function, 
                                                            _argument_function;
  bool _lazy_arguments, _caller_lazy_arguments;
  
  std::map<int, std::string> _function_analyzed_map; //Id, Function Name
  std::map<int, std::string> _function_caller_map; //Id, Function Name
  std::map<int, std::string> _function_called_map; //Id, Function Name
  std::map<int, int> _has_function_as_arguments; //Id, If has lazy arguments
  std::map<int, int> _has_function_value_as_arguments; //Id, If an value can
                                                        //be lazy
  std::map<std::string, int> _has_not_arguments_post_dom;
  //NameOfCallerFunction, HasArgumentsThatDoesn'tPostDomEntry

  public:
	void dump_csv(std::map<int, std::string> &analyzed,
                std::map<int, std::string> &caller,
                std::map<int, std::string> &called,
                std::map<int, int> &f_argument,
                std::map<int, int> &v_argument,  
                std::map<std::string, int> &pd_argument,
                std::string functionName);

  void dump_summary_csv(std::string fileName,int n_functions, int n_call,
                          int value_opportunity, int function_opportunity);
  
  //The main ideia is to print a csv file with the follow columns:
  //Id, FunctionName, FunctionCallerName, FunctionCalledName,
  //HasFunctionAsArguments, HasValueFunction
  //
  //And other csv file that resumes the content of the program
  //NameOfFunction,NumberOfFunctionsAnalyzed, NumberOfCalls, OpportunitiesValues, 
  //                                                     OpportunitiesFunctions
	

static char ID;
LazyProfitability() : FunctionPass(ID){}

void getAnalysisUsage(AnalysisUsage &AU) const;
virtual bool runOnFunction(Function &F);
};

#endif
