#include "LazyProfitability.h"

//The main ideia is to print a csv file with the follow columns:
  //Id, FunctionName, FunctionCallerName, FunctionCalledName,
  //HasFunctionAsArguments, HasValueFunction
  //
  //And other csv file that resumes the content of the program
  //NameOfFunction,NumberOfFunctionsAnalyzed, NumberOfCalls, OpportunitiesValues, 
  //                                                     OpportunitiesFunctions
	

//Imprime cada função analisada, cada função chamada e se há possibilidade
//dessa função ser lazyficada
void dump_csv(std::map<int*, std::string> &analyzed,
      std::map<int*, std::string> &caller, std::map<int*, std::string> &called,
      std::map<int*, int> &f_argument,std::map<int*, int> &v_argument,
      std::map<std::string, int> &pd_argument, std::string functionName){

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
void dump_summary_csv(std::string functionName,
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

