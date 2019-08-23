#include <iostream>
#include <map>
#include <vector>
#include <fstream>
//#include <cstring>
//#include <sstream>

class PrintCSV{
  public:

    void dump_summary_csv(std::string Benchmark, 
     std::map<std::pair <int, std::string>, std::pair<int, int>> function_map);
    void dump_DFS_csv(std::string Benchmark, int num_functions, int num_calls, 
                                             int num_args,  int num_args_lazy);
};
