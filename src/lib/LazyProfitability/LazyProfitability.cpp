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
  AU.setPreservesAll();
}

void LazyProfitability::case_1_Value_as_Argument_Load(Use *argOp, 
   int _function_value_used,std::map<Value*, Value*> *_stored_value_function){

  //Reconhece se um dos argumentos é resultado de uma função
  if(auto *value = dyn_cast<LoadInst>(argOp)){
    auto it = _stored_value_function->find(value->getOperand(0));
    if(it != _stored_value_function->end()){
      if(Instruction *inst = dyn_cast<Instruction>(it->second)){
        inst->dump();
        if(CallInst *inside_call = dyn_cast<CallInst>(inst)){
          this->_function_value_used++;
          this->_value_opportunity++;

          this->_has_function_value_as_arguments.insert(std::pair<int*, int>
                             (this->_id_function, this->_function_value_used)); 
        }
      }
    }
  }
}

void LazyProfitability::case_1_Value_as_Argument_Store(Instruction *I, 
                             std::map<Value*, Value*> *_stored_value_function){
  //Reconhece valores resutlantes de funções que foram salvos em uma
  //variavel e inserem no map 1. A variavel 2. Achamada da função
  if(StoreInst *Store = dyn_cast<StoreInst>(I)){
    Value *v = Store->getValueOperand();
    if(isa<Instruction>(v)){
      if(Instruction *inst = cast<Instruction>(v)){
        inst->dump();
        if(CallInst *call = dyn_cast<CallInst>(inst)){
          _stored_value_function->insert(std::pair<Value*, Value*>
                                 (Store->getOperand(1), Store->getOperand(0)));
        }
      }
    }   
  }
}

void LazyProfitability::case_2_Function_as_Agument(Use* argOp){
  //Reconhece se um dos argumentos é uma função
  if(auto fn = dyn_cast<Function>(argOp)){
    _called_function = fn->getName();
    //Associa o nome da função ao id da funçao analizada
    _function_called_map.insert(std::pair<int*, std::string> 
                                       (this->_id_function, _called_function));
    _call_function++; 
    _function_opportunity++;

    //Associa o número de argumentos que são funções ao id da função chamada
    _has_function_as_arguments.insert(std::pair<int*, int>
                                         (this->_id_function, _call_function));
  }
  if(auto *regCall = dyn_cast<CallInst>(argOp)){
    auto *fn = regCall->getCalledFunction(); 
    //Associa o nome da função ao id da funçao analizada
    _function_called_map.insert(std::pair<int*, std::string>
                                       (this->_id_function, _called_function));
    _call_function++; 
    _function_opportunity++;

    //Associa o número de argumentos que são funções ao id da função chamada
    _has_function_as_arguments.insert(std::pair<int*, int>
                                         (this->_id_function, _call_function));
  }
}



bool LazyProfitability::runOnModule(Module &M){ 
  int _args = 0, _lazy_args = 0, _functions = 0;
  std::string _benchmark =  M.getModuleIdentifier();
  PrintCSV *PrintCSV;
/*
  for (Function &F : M){
    for (Instruction &I : instructions(F)){
      if (CallInst *call = dyn_cast<CallInst>(&I)){
        for (unsigned i = 0; i < call->getNumOperands(); i++){
          Value *v = call->getOperand(i);
          if (isa<Instruction>(v)){
            errs() << "achou istrucao " << *v << "\n";
          }
        }
      }
    }
  }
*/
  return false;

  for(Function &F : M){
    if(!F.isDeclaration()){
      auto PDT = &getAnalysis<PostDominatorTreeWrapperPass>(F).getPostDomTree();
      PostDom *PD = new PostDom();
      PD->set_entry(&F.getEntryBlock());
   
      std::map<Value*, Value*> _stored_value_function;
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

        case_1_Value_as_Argument_Store(&I, &_stored_value_function);
        //Reconhece se a instrução é uma chamada de função
        if(CallInst *Call = dyn_cast<CallInst>(&I)){
          _analyzed_function = F.getName();

          //Pega o nome da função que está sendo analisada e que possui essa instrução
          _function_analyzed_map.insert(std::pair<int*, std::string>
                                   (this->_id_function, _analyzed_function));

          //Pega a função que está sendo chamada por essa instrução
          Function *Caller = Call->getCalledFunction();
          _call_function = 0;
          _function_value_used = 0;	

          //Se ela for uma função com nome
          if(Caller != NULL){
            if(Caller->hasName()){
              //Associa o nome da função chamada com seu id
              _caller_function = Caller->getName();
              _function_caller_map.insert(std::pair<int*, std::string>
                                      (this->_id_function, _caller_function));
              //Verifica se a função chamada possui outra funçao como argumento
              for( auto argOp = Call->arg_begin(); argOp != Call->arg_end();
                                                                      ++argOp){
                case_1_Value_as_Argument_Load(argOp, _function_value_used, 
                                                     &_stored_value_function);
                case_2_Function_as_Agument(argOp);
              }
              _n_call++;
            }           
          }	

          this->_id_function++;
          this->_n_functions++;
        }
      }
    }

    int _argument_opportunity = 0;
    for(int i = 0; i < _function_args.size(); i++){
      bool _postDom = PD->VariablePostDominates(PDT, PD->get_entry(), 
                                   PD->get_usesOfVariable(_function_args[i]));
  //    errs() << _function_args[i]->getName() << "\n";
      if(_postDom)
        _argument_opportunity++;
  }
    _functions++;
    _args += _function_args.size();
    _lazy_args += _argument_opportunity;

    _arguments_not_post_dom.insert(std::pair<std::string, int>(F.getName(),
                                                       _argument_opportunity));
/*    errs() << "-----------------------------------\n";
    errs() << "Function Arguments Analyze: \n";
    errs() << "Name: " << F.getName() << "\n";
    //errs() << "Arguments Opportunities: " << _argument_opportunity << "\n";
    errs() << "Número de argumentos que a função possui:" << _function_args.size() \
      << "\n";
    errs() << "Número de argumentos que pós-dominam a entrada: " << 
      _function_args.size() - _argument_opportunity << "\n"; 
    errs() << "Número de argumentos que podem ser Lazyficados: " <<
      _argument_opportunity << "\n";
    errs() << "-----------------------------------\n\n";
*/
    //  errs() << "Analyzed: " << _function_analyzed_map.size() << "\n";
    //  errs() << "Caller: " << _function_caller_map.size() << "\n";
    //  errs() << "Called: " << _function_called_map.size() << "\n";
    //  errs() << "Func_Arg: " << _has_function_as_arguments.size() << "\n";
    //  errs() << "Func_Value: " << _arguments_not_post_dom.size() << "\n\n"; <- ERRADO

    //  errs() << "FuncOp: " << _function_opportunity << "\n";
    //  errs() << "ValueOp: " << _value_opportunity << "\n\n";

    //dump_csv(_function_analyzed_map, _function_caller_map, _function_called_map, 
    // _has_function_as_arguments, _has_function_value_as_arguments,
    // _arguments_not_post_dom, F.getName());

    //  dump_summary_csv(F.getName(),_n_functions, _n_call, 
    //                   _value_opportunity, _function_opportunity);
    }
  } 
   PrintCSV->dump_post_dom_csv(_benchmark, _functions, _args, _lazy_args);
  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
      "Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
      false, false);
