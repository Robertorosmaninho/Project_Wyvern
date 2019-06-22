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

void LazyProfitability::dump_csv(int n_functions, std::string fileName, 
                std::map<int,std::string> &caller, 
                std::map<int, std::string> &called, 
                std::map<int, std::string> &argument){

  //Define nome do arquivo
  std::ofstream _file;
  _file.open("Details.csv");	

  //Escrevendo o cabeçalho do arquivo	
  _file << "FunctionId,FunctionAnalyzed,CallerFunctionName,hasFunctionCalled\n";

  for(auto it=caller.begin(), itC=called.begin(), itA=argument.begin(); 
                  it != caller.end(); ++it){
    _file << it->first << "," << it->second;

    if(itC != called.end() && (it->first == itC->first) ){			
      _file << "," << itC->second;
      itC++;
    }

    if(itA != argument.end() && (it->first == itA->first) ){
      _file << "," << itA->second;
      itA++;
    }
    _file << "\n";
  }
  _file.close();
}

void LazyProfitability::dump_summary_csv(std::string fileName, int n_functions, 
                                         int opportunity){
  //Define nome do arquivo
  std::ofstream _file;
   _file.open("Summary.csv");	
    _file << "ProgramName,FunctionsAnalyzed,Opportunities\n";
    _file << fileName << "," << n_functions << "," << opportunity << "\n";
  _file.close();
}
bool LazyProfitability::runOnModule(Module &M){
  for(Function &F : M){	
    std::map<Value*, Value*> _stored_value_function;
    for(BasicBlock &BB : F){
      for(Instruction &I : BB){
       
        //Reconhece valores resutlantes de funções que foram salvos em uma
        //variavel e inserem no map 1. A variavel 2. Achamada da função
        if(StoreInst *Store = dyn_cast<StoreInst>(&I)){
          Value *v = Store->getValueOperand();
          if(isa<Instruction>(v)){
            if(Instruction *inst = cast<Instruction>(v)){
              if(CallInst *call = dyn_cast<CallInst>(inst)){
                _stored_value_function.insert(std::pair<Value*, Value*>
                                 (Store->getOperand(1), Store->getOperand(0)));
              }
            }
          }   
        }

        //Reconhece se a instrução é uma chamada de função
        if(CallInst *Call = dyn_cast<CallInst>(&I)){
          _caller_function = F.getName();

          //Pega o nome da função que está sendo analisada e que possui essa instrução
         	_caller_functions_map.insert(std::pair<int, std::string>
                                             (_id_function, _caller_function));

          //Pega a função que está sendo chamada por essa instrução
          Function *Called = Call->getCalledFunction();
          int _call_function = 0, _function_value_used = 0;;	

          //Se ela for uma função com nome
          if(Called != NULL){
            if(Called->hasName()){
              //Associa o nome da função chamada com seu id
              _called_function = Called->getName();
              _called_functions_map.insert(std::pair<int, std::string>
                                              (_id_function, _called_function));
              //Verifica se a função chamada possui outra funçao como argumento
              for( auto argOp = Call->arg_begin(); argOp != Call->arg_end();
                                                                      ++argOp){
                if(auto fn = dyn_cast<Function>(argOp)){
                  _call_function++; 
                  _opportunity++;
                }
                if(auto *value = dyn_cast<LoadInst>(argOp)){
                  std::map<Value*, Value*>::iterator it;
                  it = _stored_value_function.find(value->getOperand(0));
                  if(it != _stored_value_function.end()){
                    if(Instruction *inst = dyn_cast<Instruction>(it->second)){
                      if(CallInst *inside_call = dyn_cast<CallInst>(inst)){
                        _function_value_used++;
                        _opportunity++
                      }
                    }
                  }
                }
              }
              //Associa o número de argumentos que são funções ao id da função chamada
              std::stringstream ss;
              ss << _call_function;
              _has_function_as_arguments.insert(std::pair<int, std::string>
                                                     (_id_function, ss.str()));
            }           
          }	

          _id_function++;
          _n_functions++;
        }
      }	
    }
  }
  dump_csv(_n_functions, M.getModuleIdentifier(), _caller_functions_map,\
                      _called_functions_map, _has_function_as_arguments);
  dump_summary_csv(M.getModuleIdentifier(),_n_functions,_opportunity);
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
