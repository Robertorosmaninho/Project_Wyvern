#ifndef SRC_LIB_LAZYPROFITABILITY_POSTDOM_H
#define SRC_LIB_LAZYPROFITABILITY_POSTDOM_H

#include <llvm/Analysis/PostDominators.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"

using namespace llvm;

  class PostDom : public FunctionPass{
    private:
      BasicBlock *_entry, *_end;
      //std::vector<BasicBlock> _usesOfVariable;
      std::map<Value*, std::vector<BasicBlock*> > *_var_uses; 
    
    public:
      PostDom();
      ~PostDom();

      void set_entry(BasicBlock *BB);
      BasicBlock* get_entry();

      void set_end(BasicBlock *BB);
      BasicBlock* get_end();
      
      void set_usesOfVarible(BasicBlock *BB, Value v);      
      std::vector<BasicBlock*>* get_usesOfVariable(Value v);

      bool VariablePostDominates(PostDominatorTree PDT, BasicBlock *entry, 
                                 std::vector<BasicBlock*> UsesOfVariable);
  };
#endif
