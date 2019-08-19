#include "lib.h"

void PrintCSV::dump_summary_csv(std::string Benchmark,
   std::map<std::pair <int, std::string>, std::pair<int, int>> function_map){
  //Define nome do arquivo
  std::ofstream _file;
  std::string name = "Summary_" + Benchmark + ".csv";

  _file.open(name);
  _file << "Id, FunctionName, NumOfArguments, NumOfLazyOpportunities\n"; 
  
  for(auto it = function_map.begin(); it != function_map.end(); ++it){
    _file << it->first.first << "," << it->first.second << "," << \
             it->second.first << "," << it->second.second << "\n";
  } 

  _file.close();
}


void PrintCSV::dump_DFS_csv(std::string Benchmark, int num_functions, 
                                              int num_args, int num_args_lazy){

  std::ofstream _file;

  Benchmark.pop_back();
  Benchmark.pop_back();
  Benchmark.pop_back();
  Benchmark.pop_back();
//  Benchmark.pop_back();
//  Benchmark.pop_back();
//  Benchmark.pop_back();

  std::string name = Benchmark + "_DFS_Result.csv";
  _file.open(name);
  _file << "Benchmark, NumOfCallFunctions, NumOfArguments, ";
  _file << "NumOfLazyOpportunities\n";
  _file << Benchmark << "," << num_functions << "," <<  num_args << "," << \
           num_args_lazy << "\n";
  _file.close();
}

