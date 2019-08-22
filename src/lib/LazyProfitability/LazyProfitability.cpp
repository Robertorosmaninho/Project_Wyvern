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
  AU.setPreservesAll();
}


bool LazyProfitability::runOnModule(Module &M){ 
  int _num_args = 0, _num_lazy_args = 0, _num_function = 0;
  std::string _benchmark =  M.getModuleIdentifier();
  PrintCSV *PrintCSV;

  for(Function &F : M){
    if(F.isDeclaration())
      continue;

    _num_function++;
    errs() << "\n=======================\n";
    errs() << "Function Name: " << F.getName() << "\n"; 

    for(BasicBlock &BB : F){
      for(Instruction &I : BB){
        if(CallInst* call = dyn_cast<CallInst>(&I)){
          int _args = 0, _lazy_args = 0;//, _calls = 0;
          //Pega a Função chamada pela CallInst
          Function *func = call->getCalledFunction();
          //Testa se é uma chamada indireta
          if(func == nullptr || func->isDeclaration() || !func->hasName())
            continue; 
          
          DepthFirstSearch* DFS = &getAnalysis<DepthFirstSearch>(F);
          /*This part run over the function to find a path that an argument 
            has no use */
          BasicBlock *bb = &func->getEntryBlock();
          errs() << " - Function Call: " << func->getName() << "\n";
          for(auto arg = func->arg_begin(); arg != func->arg_end(); ++arg){
            //errs() << arg->getName() << "\n";
            DFS->addBasicBlock(bb);
            DFS->reset_visited();
            bool result = DFS->hasBlockWithoutUse(arg, bb);
            _args++;
            if(result){
              _lazy_args++;
              errs() << " --> Lazy: " <<arg->getName() << "\n";
            }
          }
          _num_args+=_args;
          _num_lazy_args+=_lazy_args;

          //Add ID, Function, num_args and num_lazy_args into a map
          _function_map.insert(std::make_pair(
                                std::make_pair(_num_calls++,func->getName()),
                                std::make_pair(_args, _lazy_args)));
          //Calculate the dependences of the function
          phoenix::ProgramDependenceGraph PDG;
          PDG.compute_dependences(func);
            
          for(unsigned i = 0; i < call->getNumOperands(); i++){
            Value *v = call->getOperand(i);
            if(Instruction* inst = dyn_cast<Instruction>(v)){
              errs() << "   - Instrução: ";
              inst->dump();
              _dependences = PDG.get_dependences_for(inst);
            /*for(auto it = _dependences.begin(); it != _dependences.end(); 
                                                                          ++it){
                Instruction *a = *it;
                a->dump();
              }*/
            }
          } 
        }
      }
    }
  }
  PrintCSV->dump_summary_csv(_benchmark, _function_map);
                                 
  PrintCSV->dump_DFS_csv(_benchmark, _num_function, _num_calls,_num_args,_num_lazy_args);
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
