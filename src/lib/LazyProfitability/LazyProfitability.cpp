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
	fileName = "Count_" + fileName;
	char str[fileName.size() + 1];
	strcpy(str, fileName.c_str());
	char * _name = strtok(str, ".");
	strcat(_name, ".csv");
 	
	_file.open(_name);	

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

bool LazyProfitability::runOnModule(Module &M){
  for(Function &F : M){	
	  for(BasicBlock &BB : F){
      for(Instruction &I : BB){
				//Reconhece se a instrução é uma chamada de função
			  if(CallInst *Call = dyn_cast<CallInst>(&I)){
          _caller_function = F.getName();

					//Pega o nome da função que está sendo analisada e que possui essa instrução
          _caller_functions_map.insert(std::pair<int, std::string>
                                              (_id_function, _caller_function));

					//Pega a função que está sendo chamada por essa instrução
			    Function *Called = Call->getCalledFunction();
					int _call_function = 0;	

					//Se ela for uma função com nome
					if(Called != NULL){
						if(Called->hasName()){
							//Associa o nome da função chamada com seu id
              _called_function = Called->getName();
              _called_functions_map.insert(std::pair<int, std::string>
                                              (_id_function, _called_function));
							//Verifica se a função chamada possui outra funçao como argumento
							for( auto argOp = Call->arg_begin(); argOp != Call->arg_end(); ++argOp){
								if(auto fn = dyn_cast<Function>(argOp)){
                  _call_function++; 
									_opportunity++;
								}
							}
						//Assodia o número de argumentos que são funções ao id da função chamada
						std::stringstream ss;
					 	ss << _call_function;
						_has_function_as_arguments.insert(std::pair<int, std::string>
														(_id_function, ss.str()));
						}           
					}	

          _id_function++;
          _n_functions++;

					errs() << _id_function << "\t\t" << _caller_function << "\t\t" \
									<< _called_function << "\t\t" << _call_function << "\n";
        }
      }	
    }
  }
	dump_csv(_n_functions, M.getModuleIdentifier(), _caller_functions_map,\
									_called_functions_map, _has_function_as_arguments);

  return false;
}

char LazyProfitability::ID = 0;
static RegisterPass<LazyProfitability> X("LazyProf", 
			"Count opportunities to apply Lazy Evaluation on a Funcion parameter", 
			false, false);

//Function->HasLazyArguments
