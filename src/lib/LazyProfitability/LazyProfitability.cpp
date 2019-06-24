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
  //AU.addRequired<PostDom>();
  AU.setPreservesAll();
}

//Imprime cada função analisada, cada função chamada e se há possibilidade
//dessa função ser lazyficada
void LazyProfitability::dump_csv(std::map<int, std::string> &analyzed, 
                                 std::map<int, std::string> &caller, 
                                 std::map<int, std::string> &called, 
                                 std::map<int, int> &f_argument,
                                 std::map<int, int> &v_argument){

  //Define nome do arquivo
  std::ofstream _file;
  _file.open("Details.csv");	

  //Escrevendo o cabeçalho do arquivo	
  _file << "Id, FunctionName, FunctionCallerName, FunctionCalledName, \
            HasFunctionAsArguments, HasValueFunction\n";

  auto itF = f_argument.begin(), itV = v_argument.begin();
  for(auto it=caller.begin(), itA=analyzed.begin(), itC=called.begin(); 
                  itA != analyzed.end(); ++itA){
    _file << it->first << "," << itA->second;
    
    if(it != caller.end() && it->first == itA->first){
      _file << "," << it->second;
      //id, FunctionAnalyzed, FunctionCaller
      it++;
    }else{
      _file << "," << " - ";
    }

    if(itC != called.end() && (itA->first == itC->first) ){			
      _file << "," << itC->second;
      itC++;
    }else{
      _file << "," << " - ";
    }

    if(itF != f_argument.end() && (itA->first == itF->first) ){
      _file << "," << itF->second;
      itF++;
    }else{
      _file << "," << 0;  
    }

    if(itV != v_argument.end() && (itA->first == itV->first)){
      _file << "," << itV->second;
      itV++;
    }else{
      _file << "," << 0;
    }

    _file << "\n";
  }
  _file.close();
}

//Imprime o nome do programa analisado e a quantidade de oportunidades para usar
//Lazy Evaluation
void LazyProfitability::dump_summary_csv(std::string fileName,int n_functions, 
                 int n_call, int value_opportunity, int function_opportunity){
  //Define nome do arquivo
  std::ofstream _file;
   _file.open("Summary.csv");	
    _file << "NameOfProgram,NumberOfFunctionsAnalyzed, NumberOfCalls,\
                            OpportunitiesValues, OpportunitiesFunctions\n";
    _file << fileName << "," << n_functions << ","  << n_call << "," << \
             value_opportunity << "," << function_opportunity << "\n";
  _file.close();
}
bool LazyProfitability::runOnModule(Module &M){
  for(Function &F : M){	
    std::map<Value*, Value*> _stored_value_function;
    for(BasicBlock &BB : F){

        errs() << "BasicBlock: "<< BB.getName() << "\n";

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
          _analyzed_function = F.getName();
          errs() << F.getName() << "\n";

          //Pega o nome da função que está sendo analisada e que possui essa instrução
         	_function_analyzed_map.insert(std::pair<int, std::string>
                                            (_id_function, _analyzed_function));

          //Pega a função que está sendo chamada por essa instrução
          Function *Caller = Call->getCalledFunction();
          _call_function = 0;
          _function_value_used = 0;	

          //Se ela for uma função com nome
          if(Caller != NULL){
            if(Caller->hasName()){
              //Associa o nome da função chamada com seu id
              _caller_function = Caller->getName();
              _function_caller_map.insert(std::pair<int, std::string>
                                              (_id_function, _caller_function));
              //Verifica se a função chamada possui outra funçao como argumento
              for( auto argOp = Call->arg_begin(); argOp != Call->arg_end();
                                                                      ++argOp){
                //Reconhece se um dos argumentos é uma função
                if(auto fn = dyn_cast<Function>(argOp)){
                  _called_function = fn->getName();
                  //Associa o nome da função ao id da funçao analizada
                  _function_called_map.insert(std::pair<int, std::string>
                                              (_id_function, _called_function));
                  _call_function++; 
                  _function_opportunity++;

                  //Associa o número de argumentos que são funções ao id da função chamada
                  _has_function_as_arguments.insert(std::pair<int, int>
                                                (_id_function, _call_function));

                }
                //Reconhece se um dos argumentos é resultado de uma função
                if(auto *value = dyn_cast<LoadInst>(argOp)){
                  std::map<Value*, Value*>::iterator it;
                  it = _stored_value_function.find(value->getOperand(0));
                  if(it != _stored_value_function.end()){
                    if(Instruction *inst = dyn_cast<Instruction>(it->second)){
                      if(CallInst *inside_call = dyn_cast<CallInst>(inst)){
                        _function_value_used++;
                        _value_opportunity++;

                        _has_function_value_as_arguments.insert(
                        std::pair<int, int>(_id_function, _function_value_used)); 
                      }
                    }
                  }
                }

                if(auto *regCall = dyn_cast<CallInst>(argOp)){
                  auto *fn = regCall->getCalledFunction(); 
                  //Associa o nome da função ao id da funçao analizada
                  _function_called_map.insert(std::pair<int, std::string>
                                              (_id_function, _called_function));
                  _call_function++; 
                  _function_opportunity++;

                  //Associa o número de argumentos que são funções ao id da função chamada
                  _has_function_as_arguments.insert(std::pair<int, int>
                                                (_id_function, _call_function));
                }
              }
              _n_call++;
            }           
          }	

          _id_function++;
          _n_functions++;
        }
      }	
    }
  }
  errs() << "Analyzed: " << _function_analyzed_map.size() << "\n";
  errs() << "Caller: " << _function_caller_map.size() << "\n";
  errs() << "Called: " << _function_called_map.size() << "\n";
  errs() << "Func_Arg: " << _has_function_as_arguments.size() << "\n";
  errs() << "Func_Value: " << _has_function_value_as_arguments.size() << "\n\n";

  errs() << "FuncOp: " << _function_opportunity << "\n";
  errs() << "ValueOp: " << _value_opportunity << "\n";

  dump_csv(_function_analyzed_map, _function_caller_map, _function_called_map, 
           _has_function_as_arguments, _has_function_value_as_arguments);

  dump_summary_csv(M.getModuleIdentifier(),_n_functions, _n_call, 
                   _value_opportunity, _function_opportunity);
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
