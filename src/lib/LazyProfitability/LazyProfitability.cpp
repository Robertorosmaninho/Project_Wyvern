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
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.addRequired<DepthFirstSearch>();
  //AU.addRequired<>
  AU.setPreservesAll();
}


bool LazyProfitability::runOnModule(Module &M){ 
  int _args = 0, _lazy_args = 0; //, _functions = 0;
 // std::string _benchmark =  M.getModuleIdentifier();
 // PrintCSV *PrintCSV;

  for(Function &F : M){
    std::vector<Value*> _function_args;
 
    if(!F.isDeclaration()){

  //    DepthFirstSearch* DFS = &getAnalysis<DepthFirstSearch>(F);
      errs() << "Args: ";
      for( auto arg = F.arg_begin(); arg != F.arg_end(); ++arg){
        _function_args.push_back(arg);
        errs() << arg->getName() << ", ";
      }
      errs() << "\n";
      for(BasicBlock &BB : F){
   //     errs() << "#########\n";
  //      errs() << BB.getName() << "\n";
        for(unsigned i = 0; i < _function_args.size(); i++)
//          errs() << DFS->hasUse(&BB, _function_args[i]) << "\n";

        for(Instruction &I : BB){
          if(CallInst* call = dyn_cast<CallInst>(&I)){
            Function *func = call->getCalledFunction();
            DepthFirstSearch* DFS = &getAnalysis<DepthFirstSearch>(F);
            /*This part run over the function to find a path that an argument 
            has no use */
            BasicBlock *bb = &func->getEntryBlock();
            errs() << "CallInst: ";
            I.dump();
            errs() << "Função: " << func->getName() << "\n";
            for(auto arg = func->arg_begin(); arg != func->arg_end(); ++arg){
             // errs() << arg->getName() << "\n";
              DFS->addBasicBlock(bb);
              bool result = DFS->hasBlockWithoutUse(arg, bb);
              _args++;
              if(result)
                _lazy_args++;
            }
            _function_map.insert(std::make_pair(
                                           std::make_pair(_id_function++,func),
                                           std::make_pair(_args, _lazy_args)));
            for(unsigned i = 0; i < call->getNumOperands(); i++){
              Value *v = call->getOperand(i);
              if(isa<Instruction>(v)){
                errs() << "Instrução: " << *v << "\n";
                
              }
            }
          }
        }
      }
    }
  } 
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
