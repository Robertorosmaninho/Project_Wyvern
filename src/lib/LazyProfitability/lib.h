#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>

class PrintCSV{
  public:
    void dump_csv(std::map<int*, std::string> &analyzed,
      std::map<int*, std::string> &caller, std::map<int*, std::string> &called,
      std::map<int*, int> &f_argument,std::map<int*, int> &v_argument,
      std::map<std::string, int> &pd_argument, std::string functionName);

    void dump_summary_csv(std::string functionName, int n_functions, 
                  int n_call, int value_opportunity, int function_opportunity);

    void dump_post_dom_csv(std::string Benchmark, int num_functions,
                                              int num_args, int num_args_lazy);
    void dump_DFS_csv(std::string Benchmark, int num_functions,
       int num_args, int num_args_lazy, int problem, int solved, int unsolved);
};
