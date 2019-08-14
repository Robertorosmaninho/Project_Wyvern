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
  int _num_args = 0, _num_lazy_args = 0, problem = 0, analyzed = 0, solved = 0;
  int unsolved = 0;
  std::string _benchmark =  M.getModuleIdentifier();
  PrintCSV *PrintCSV;
  std::map<std::pair<std::string,Type*>, Function*> functions;
  int w = 0;
  for(Function &F : M){
    functions.insert(std::make_pair(std::make_pair(F.getName(), F.getType()), &F));
    std::vector<Value*> _function_args;
    if(F.isDeclaration())
      continue;
    errs() << "\n=======================\n";
    errs() << "Function Name: " << F.getName() << "\n"; 
    for(BasicBlock &BB : F){
      for(Instruction &I : BB){
        if(CallInst* call = dyn_cast<CallInst>(&I)){
          call->dump();
          if (call)
            if (call->getCalledFunction()) {
              call->getCalledFunction()->dump();
            } 
            else {
              errs() << "BUG!\n";
            }   
          int _args = 0, _lazy_args = 0;
          analyzed++;
          //Pega a Função chamada pela CallInst
          Function *func = call->getCalledFunction();
          //Testa se é uma chamada indireta
          if(func == nullptr){
            Value* val = call->getCalledValue();
            //Sendo uma chamada indireta procura a função na lista de f já analisadas
            auto it = functions.find(std::make_pair(val->getName(), val->getType()));
//            if(it != functions.end()){
  //            solved++;
    //          func = it->second;
     //       }else{
            //Senão achar a função, acredito que seja um @atoi, @getelementptr
            //que não precisam ser analisados pelo nosso passe
            //Error: Algumas funções que são chamadas por ponteiros para função não              são encontradas.
             I.dump();
             errs() << "This Call is an IndirectCall, so we can't get the\
 function associeated\n";
            unsolved++;
            continue; 
//            }   
            problem++;
          }
         if(func->isDeclaration())
            continue;

          DepthFirstSearch* DFS = &getAnalysis<DepthFirstSearch>(F);
            /*This part run over the function to find a path that an argument 
            has no use */
          BasicBlock *bb = &func->getEntryBlock();
          errs() << " - Function Call: " << func->getName() << "\n";
          for(auto arg = func->arg_begin(); arg != func->arg_end(); ++arg){
           // errs() << arg->getName() << "\n";
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
                                           std::make_pair(_id_function++,func),
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
 // PrintCSV->dump_post_dom_csv(_benchmark, _id_function+1, _num_args, 
 //                                                              _num_lazy_args);
 //   PrintCSV->dump_DFS_csv(_benchmark, _id_function, _num_args,_num_lazy_args, problem);
  PrintCSV->dump_DFS_csv(_benchmark, analyzed, _num_args,_num_lazy_args, problem, solved, unsolved);
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
