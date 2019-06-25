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
#include "PostDom.h"
  
void LazyProfitability::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.setPreservesAll();
}

//Imprime cada função analisada, cada função chamada e se há possibilidade
//dessa função ser lazyficada
void LazyProfitability::dump_csv(std::map<int, std::string> &analyzed, 
                                 std::map<int, std::string> &caller, 
                                 std::map<int, std::string> &called, 
                                 std::map<int, int> &f_argument,
                                 std::map<int, int> &v_argument,
                                 std::map<std::string, int> &pd_argument,
                                 std::string functionName){

  //Define nome do arquivo
  std::ofstream _file;
  std::string name  = "Details_" + functionName + ".csv";
  _file.open(name);	

  //Escrevendo o cabeçalho do arquivo	
  _file << "Id, FunctionName, FunctionCallerName, FunctionCalledName, \
            HasFunctionAsArguments, HasValueFunction, HasPostDomArguments\n";

  auto itF = f_argument.begin(), itV = v_argument.begin(); 
  auto itPD = pd_argument.end(); itPD--;
  for(auto it=caller.begin(), itA=analyzed.begin(), itC=called.begin(); 
                                                itA != analyzed.end(); ++itA){
    //id, FunctionAnalyzed
    _file << it->first << "," << itA->second;
    
    if(it != caller.end() && it->first == itA->first){
      _file << "," << it->second;
     //FunctionCallerName
      it++;
    }else{
      _file << "," << " - ";
    }

    if(itC != called.end() && (itA->first == itC->first) ){			
      _file << "," << itC->second;
      //FuncitonCalledName
      itC++;
    }else{
      _file << "," << " - ";
    }

    if(itF != f_argument.end() && (itA->first == itF->first) ){
      _file << "," << itF->second;
      //HasFunctionAsArguments
      itF++;
    }else{
      _file << "," << 0;  
    }

    if(itV != v_argument.end() && (itA->first == itV->first)){
      _file << "," << itV->second;
      //HasValueFunction
      itV++;
    }else{
      _file << "," << 0;
    }

    errs() << "PD: " << itPD->first << "\nF_name: " << itA->second << "\n";
    if(itPD != pd_argument.begin() && (itPD->first == itA->second)){
      _file << "," << itPD->second;
      //HasUsesofArgumentsThatPostDomTheEntry
      itPD--; 
    }else{
      _file << "," << 0;
    }
    _file << "\n";
  }
  _file.close();
}

//Imprime o nome do programa analisado e a quantidade de oportunidades para usar
//Lazy Evaluation
void LazyProfitability::dump_summary_csv(std::string functionName,
  int n_functions, int n_call, int value_opportunity, int function_opportunity){
  //Define nome do arquivo
  std::ofstream _file;
  std::string name = "Summary_" + functionName + ".csv";

   _file.open(name);	
    _file << "NameOfFunction,NumberOfFunctionsAnalyzed, NumberOfCalls,\
                            OpportunitiesValues, OpportunitiesFunctions\n";
    _file << functionName << "," << n_functions << ","  << n_call << "," << \
             value_opportunity << "," << function_opportunity << "\n";
  _file.close();
}
bool LazyProfitability::runOnFunction(Function &F){
  auto PDT = &getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
  PostDom *PD = new PostDom();

  std::map<Value*, Value*> _stored_value_function;

  PD->set_entry(&F.getEntryBlock());

 std::vector<Value*> _function_args;

  for( auto arg = F.arg_begin(); arg != F.arg_end(); ++arg) 
    _function_args.push_back(arg);

  for(BasicBlock &BB : F){
    for(Instruction &I : BB){
      //Reconhece os argumentos da função e busca o uso de cada uma no bloco básico      
      for(auto Op = I.op_begin(); Op != I.op_end(); ++Op){
        for(int i = 0; i <  _function_args.size(); i++){
          if(Op->get()->hasName() && _function_args[i]->hasName()){
            std::string name = _function_args[i]->getName();
            std::string name_addr = name + ".addr"; 
            
            std::string OpName = Op->get()->getName();
            if( OpName == name || OpName == name_addr){
              PD->set_usesOfVarible(&BB, _function_args[i]);
            }
          }
        }
      }

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
  //      errs() << F.getName() << "\n";

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
  int _argument_opportunity = 0;
  for(int i = 0; i < _function_args.size(); i++){
    bool _postDom = PD->VariablePostDominates(PDT, PD->get_entry(), 
                                   PD->get_usesOfVariable(_function_args[i]));
    if(_postDom)
      _argument_opportunity++;
  }

  _has_not_arguments_post_dom.insert(std::pair<std::string, int>(F.getName(),
                                                       _argument_opportunity));
  errs() << "-----------------------------------\n";
  errs() << "Function Arguments Analyze: \n";
  errs() << "Name: " << F.getName() << "\n";
  errs() << "Arguments Opportunities: " << _argument_opportunity << "\n";
  errs() << "-----------------------------------\n\n";

  errs() << "Analyzed: " << _function_analyzed_map.size() << "\n";
  errs() << "Caller: " << _function_caller_map.size() << "\n";
  errs() << "Called: " << _function_called_map.size() << "\n";
  errs() << "Func_Arg: " << _has_function_as_arguments.size() << "\n";
  errs() << "Func_Value: " << _has_function_value_as_arguments.size() << "\n\n";

  errs() << "FuncOp: " << _function_opportunity << "\n";
  errs() << "ValueOp: " << _value_opportunity << "\n\n";

  dump_csv(_function_analyzed_map, _function_caller_map, _function_called_map, 
     _has_function_as_arguments, _has_function_value_as_arguments,
     _has_not_arguments_post_dom, F.getName());

  dump_summary_csv(F.getName(),_n_functions, _n_call, 
                   _value_opportunity, _function_opportunity);
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
