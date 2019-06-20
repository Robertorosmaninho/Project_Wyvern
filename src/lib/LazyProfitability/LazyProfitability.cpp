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
  errs() << "FunctionId"<< "\t" << "CallerFunctionName" << "\t" << \
						"CalledFunctionName" << "\n"; 
  for(Function &F : M){	
	  for(BasicBlock &BB : F){
      for(Instruction &I : BB){
			  if(CallInst *Call = dyn_cast<CallInst>(&I)){
          _caller_function = F.getName();

          _caller_functions_map.insert(std::pair<int, std::string>
                                              (_id_function, _caller_function));

			    Function *Called = Call->getCalledFunction();
					

					if(Called != NULL){
						if(Called->hasName()){
              _called_function = Called->getName();
              _called_functions_map.insert(std::pair<int, std::string>
                                              (_id_function, _called_function));
int k = 0;
							for( auto argOp = Call->arg_begin(); argOp != Call->arg_end(); ++argOp){
								errs() << _called_function << " : " << k << " - "<< \
												Call->getArgOperand(k)->getName() << "\n";
								if(auto fn = dyn_cast<Function>(argOp))
									errs() << "peguei uma função\n";
								k++;
							}
              for(auto arg = Called->arg_begin(); arg != Called->arg_end(); ++arg) {
                  errs() << *arg << "\n";
									if(auto fn = dyn_cast<Function>(arg))
										errs() << "peguei uma função\n";
              }
						}
            
					}	

          _id_function++;
          _n_functions++;

					errs() << _id_function << "\t\t" << _caller_function << "\t\t" \
									<< _called_function << "\n";
        }
      }	
    }
  }
	errs() << "----------------------------------------------------------------\
-----------------\n";
	errs() << "Number of Functions: " << _n_functions << "\n";
	//errs() << "Number of Caller Functions: " << << "\n";
	errs() << "Number of Opportunities: " << _opportunity << "\n";
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
			"Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
			false, false);

//Function->HasLazyArguments
